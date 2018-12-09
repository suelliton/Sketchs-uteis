#include <Ultrasonic.h>
Ultrasonic ultrasonic(43,45); // (Trig PIN,Echo PIN)
#include <Servo.h>
Servo myservo;




int led_perdido = 22;
int led_achou = 23;
int led_ancorado = 24;
int led_dentro_horta = 25;
int led_fora_horta = 26;



int ligaDireito = 11;
int frenteDireito = 9;
int trasDireito = 10;

int ligaEsquerdo= 6;
int frenteEsquerdo = 8;
int trasEsquerdo = 7;

int percentualGraddle = 30;


//estados: perdido = 0 = BRANCO  achou = 1 = VERDE BRILHANTE   ancorado = 2 = AZUL   dentroHorta = 3 = VERDECLARO    foraHorta= 4  = VERMELHO
//acoes: frente = 0 , direita = 1, esquerda= 2, tras = 3, parar = 4
/*                                 frente 100| direita 100 | esquerda 100 | tras 100 | frente 300| direita 300 | esquerda 300 | tras 300 |frente 700| direita 700 | esquerda 700 | tras 700 | 
 *                                  frente 1000| direita 1000 | esquerda 1000 | tras 1000 |
perdido                          |    0       0         0        0         
achou                            |    0       0         0        0         
ancorado                         |    0       0         0        0         
dentroHorta                      |    0       0         0        0         
foraHorta                        |    0       0         0        0       

*/
float Q[5][16] = {0};
float R[5][16] = {0 };
int qtd_episodios = 50 ;

void setup() {
          Serial.begin(9600);
          myservo.attach(12);
          myservo.write(90);
          
          pinMode(led_perdido,OUTPUT);
          pinMode(led_achou,OUTPUT);
          pinMode(led_ancorado,OUTPUT);
          pinMode(led_dentro_horta,OUTPUT);
          pinMode(led_fora_horta,OUTPUT);
       
          
          pinMode(ligaDireito,OUTPUT);
          pinMode(frenteDireito,OUTPUT);
          pinMode(trasDireito,OUTPUT);        
          pinMode(ligaEsquerdo,OUTPUT);
          pinMode(frenteEsquerdo,OUTPUT);
          pinMode(trasEsquerdo,OUTPUT);

          
          digitalWrite(ligaDireito,HIGH);//ativa o lado dieito  
          digitalWrite(ligaEsquerdo,HIGH);//stivs o lado esquerdo
          

          
          randomSeed(analogRead(A3));
}





void loop(){              
              
        
        
         int state = 0;// inicializa variavel estado           
         int acao = 0;//inicializa variavel acao         
         float aprendizagem = 0.8;
         float fatorD = 0.95;
         
      for(int i = 0; i < qtd_episodios ; i++ ){ // 20 episódios                 
          state = verificaEstado(); // verifica o estado atual do agente
          while(state != 1){
               int gradle = random(101);
               int aRandom = random(4);
              acao = escolheAcao(state,gradle,aRandom);
               
              executaAcao(acao);   
                    
             // delay(300);            
              
              parar();
              int slinha = verificaEstado();
              exibeValores(state,acao);
              
              Q[state][acao] = Q[state][acao] + aprendizagem * (R[state][acao]+ geraRecompensa(slinha) + (fatorD * (Q[slinha][acao] - Q[state][acao])));
              
              state = slinha;            
              delay(0);
          }          
        delay(10000);
          percentualGraddle = percentualGraddle - 5;
      }

    while(true){
      for(int i=0;i<5;i++){
      acendeLed(i);
      delay(200);
      }
    }
 }


 float geraRecompensa(int s){  
  switch(s){
    case 0:
      return -5;//perdido
    break;
    case 1:
      return 10;//achou
    break;
    case 2:
      return 5;//ancorado
    break;
    case 3:
      return -3;//dentro
    break;
    case 4:
      return -3;//fora
    break;
    default:
    break;
    }

    /*
  if(s==1 || s == 2){
    //return (analogRead(A0)/950) + 1;
      return ((1024 - analogRead(A0))/1000)+1;
   }else{
    //return analogRead(A0)/950;
      return (1024 - analogRead(A0))/1000;
   }*/
   return 0;
  }

void executaAcao(int a){
      switch(a){
            case 0:
                frente(255);
                delay(100);
            break;
            case 1:
                giraDireita(255);
                delay(100);
            break;
            case 2:
                giraEsquerda(255); 
                delay(100);
            break;
            case 3:
                 tras(255);
                 delay(100);
            break;
            case 4:
                frente(255);
                delay(300);
            break;
            case 5:
                giraDireita(255);
                delay(300);
            break;
            case 6:
                giraEsquerda(255); 
                delay(300);
            break;
            case 7:
                tras(255);
                delay(300);
            break;
            case 8:
                frente(255);
                delay(700);
            break;
            case 9:
                giraDireita(255);
                delay(700);
            break;
            case 10:
                 giraEsquerda(255); 
                delay(700);
            break;
            case 11:
                tras(255);
                delay(700);
            break;
            case 12:
                frente(255);
                delay(1100);
            break;
            case 13:
                giraDireita(255);
                delay(1100);
            break;
            case 14:
                giraEsquerda(255); 
                delay(1100);
            break;
            case 15:
                tras(255);
                delay(1100);
            break;
            
        }
     
  
}
int escolheAcao(int s, int gradle,int aRandom){   
   
   float vetor[16]= {0,0,0,0};        
   int a = 0;
   for(int i=0; i < 16; i++){ //percorre as ações do estado colocando num vetor
   vetor[i] = Q[s][i]; // atribui a linha referente ao estado s  
   Serial.print("vetor");
Serial.println(vetor[i]); 
   } 
   
   int iMaior = 0;
   int indiceAnterior=0;
   int i = 0; 
   do {
        if (vetor[i] > vetor[indiceAnterior]) {
            iMaior = i;
            indiceAnterior = i;//in
        }     
        i++;
    } while (i < 16);
    
  if(iMaior == 0 || gradle < percentualGraddle && gradle >= 0 ){ //se o maior == 0 ou o gradle aleatorio for verdadeiro o agente deve explorar    
    Serial.println("Aleatória!!");
      a =  aRandom ;
  }else{
      Serial.println("Acao escolhida!!"); 
      a = iMaior;// a acao recebe o maior valor da linha do estado atual da matriz q
  }      
  Serial.println(a);
  return a;
}

int verificaEstado(){
     int ultrassom = ultrasonic.Ranging(CM); 
  
  
   if(analogRead(A0) > 990){
    Serial.println("achou");
    acendeLed(1);
    return 1; // retorna que alcançou o objetivo 
   }else if(ultrassom > 30 ){ 
    acendeLed(0);
    Serial.println("perdido");   
    return 0;
   }else if(ultrassom <= 10){
    acendeLed(4);
    Serial.println("dentroHorta");   
    return 3;
   }else if(ultrassom > 10 && ultrassom <= 20){
    acendeLed(2);
    Serial.println("ancorado");   
    return 2; 
   }else if(ultrassom > 20 && ultrassom <=30){
    acendeLed(3);
    Serial.println("foraHorta");   
    return 4;
   }  
    
 }
 


void exibeValores(int s, int a){
    Serial.print("luz :  ");
    Serial.print(analogRead(A0)); 
    Serial.print("    ultrassom : ");
    Serial.println(ultrasonic.Ranging(CM)); 
    Serial.print("estado : ");
    Serial.print(s);
    Serial.print("    acao : ");
    Serial.println(a);  
  //exibe Q
   for(int i=0;i < 5;i++){
      for(int j=0;j < 4;j++){
        Serial.print(Q[i][j]);
        Serial.print("      ");
      }  
      Serial.println("");
    }  
  } 

 
void parar(){  
  digitalWrite(trasDireito,LOW);  
  digitalWrite(frenteDireito,LOW);
  digitalWrite(trasEsquerdo,LOW);  
  digitalWrite(frenteEsquerdo,LOW);
  }
void frente(int vel){
  digitalWrite(trasDireito,LOW);//desativa os opostos
  digitalWrite(trasEsquerdo,LOW);
  analogWrite(frenteDireito,vel);  
  analogWrite(frenteEsquerdo,vel);  
  }
void tras(int vel){  
  digitalWrite(frenteDireito,LOW);
  digitalWrite(frenteEsquerdo,LOW);
  analogWrite(trasDireito,vel); 
  analogWrite(trasEsquerdo,vel); 
  }
 void giraEsquerda(int vel){
  digitalWrite(trasDireito,LOW);
  digitalWrite(frenteEsquerdo,LOW);
  analogWrite(frenteDireito,vel); 
  analogWrite(trasEsquerdo,vel);
  } 
  void giraDireita(int vel){
  digitalWrite(frenteDireito,LOW);
  digitalWrite(trasEsquerdo,LOW);
  analogWrite(trasDireito,vel); 
  analogWrite(frenteEsquerdo,vel);
  } 
  void acendeLed(int estado){
  if(estado == 0){
      digitalWrite(led_perdido,HIGH);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,LOW);
      digitalWrite(led_dentro_horta,LOW);     
      digitalWrite(led_fora_horta,LOW);
    }else if(estado == 1){
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,HIGH);
      digitalWrite(led_ancorado,LOW);
      digitalWrite(led_dentro_horta,LOW);     
      digitalWrite(led_fora_horta,LOW);
    }else if(estado == 2){
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,HIGH);
      digitalWrite(led_dentro_horta,LOW);     
      digitalWrite(led_fora_horta,LOW);
    }else if(estado == 3){      
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,LOW);
      digitalWrite(led_dentro_horta,HIGH);     
      digitalWrite(led_fora_horta,LOW);
      }else if(estado == 4){        
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,LOW);
      digitalWrite(led_dentro_horta,LOW);     
      digitalWrite(led_fora_horta,HIGH);
        }else if(estado == 5){        
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,HIGH);
      digitalWrite(led_dentro_horta,HIGH);     
      digitalWrite(led_fora_horta,LOW);
        }else if(estado == 6){        
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,LOW);
      digitalWrite(led_dentro_horta,HIGH);     
      digitalWrite(led_fora_horta,HIGH);
        }else if(estado == 7){        
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,HIGH);
      digitalWrite(led_dentro_horta,LOW);     
      digitalWrite(led_fora_horta,HIGH);
        }else if(estado == 8){        
      digitalWrite(led_perdido,LOW);
      digitalWrite(led_achou,LOW);
      digitalWrite(led_ancorado,HIGH);
      digitalWrite(led_dentro_horta,HIGH);     
      digitalWrite(led_fora_horta,HIGH);
        }
        
  }

  
  

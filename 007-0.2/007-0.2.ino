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
int frenteDireito =  7;
int trasDireito = 8;

int ligaEsquerdo= 6;
int frenteEsquerdo = 10;
int trasEsquerdo = 9;

int percentualGraddle = 30;


//estados: perdido = 0 = BRANCO  achou = 1 = VERDE BRILHANTE   ancorado = 2 = AZUL   dentroHorta = 3 = VERDECLARO    foraHorta= 4  = VERMELHO
//acoes: frente = 0 , direita = 1, esquerda= 2, tras = 3, parar = 4
/*                                 frente| direita | esquerda |
perdido                          |    0       0         0                 
achou                            |    1       0         0                 
ancorado                         |    1       0         0                 
dentroHorta                      |    0       0         0                 
foraHorta                        |    0       0         0          
state5                           |    0       0         0        
state6                           |    0       0         0         
state7                           |    0       0         0        
state8                           |    0       0         0          
state9                           |    0       0         0          
state10                          |    0       0         0          
state11                          |    0       0         0          
state12                          |    0       0         0          
state13                          |    0       0         0          
state14                          |    0       0         0   

*/
float Q[15][4] = {0};
float R[15][4] = {0,0,0,0,
                 10,10,10,10,
                 -1,0,0,1,
                 -1,0,0,1,
                 -1,0,0,1,
-0.1,-0.1,-0.1,-0.1,
-0.2,-0.2,-0.2,-0.2,
-0.3,-0.3,-0.3,-0.3,
-0.4,-0.4,-0.4,-0.4,
-0.5,-0.5,-0.5,-0.5,
-0.6,-0.6,-0.6,-0.6,
-0.7,-0.7,-0.7,-0.7,
-0.8,-0.8,-0.8,-0.8,
-0.9,-0.9,-0.9,-0.9

};
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
               int aRandom = random(3);
              acao = escolheAcao(state,gradle,aRandom);
               
              executaAcao(acao);   
                    
              delay(500);            
              
              parar();
              int slinha = verificaEstado();
              exibeValores(state,acao);
              
              Q[state][acao] = Q[state][acao] + aprendizagem * (R[state][acao]+ geraRecompensa(slinha) + (fatorD * (Q[slinha][acao] - Q[state][acao])));
              
              state = slinha;            
              delay(0);
          }          
        delay(10000);
          percentualGraddle = percentualGraddle - 3;
      }

    while(true){
      for(int i=0;i<5;i++){
      acendeLed(i);
      delay(200);
      }
    }
 }


 float geraRecompensa(int s){  
  int infra = analogRead(A0);
  switch(s){
    case 0:
      return (1024 - infra)/1024;//perdido      
    break;
    case 1:
      return (1024 - infra)/1024;//achou
    break;
    case 2:
      return -(1024 -infra)/1024;//ancorado
    break;
    case 3:
      return -(1024 -infra)/1024;//dentro
    break;
    case 4:
      return - (1024 -infra)/1024;//fora
    break;
    default:
    return (1024 -infra)/1024;
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
      if(a == 0){
          frente(255);
          //delay(200);
      }else if(a == 1){
          giraDireita(255);
          //delay(200);
          //parar();
          //frente(255);
          //delay(200);
      }else if(a == 2){        
          giraEsquerda(255);  
          //delay(200);
          //parar();
          //frente(255);
          //delay(200);
      }else if(a == 3){
          tras(255);
          //delay(200);
      }else if(a == 4){
          parar();  
      }
  
}
int escolheAcao(int s, int gradle,int aRandom){   
   
   float vetor[4]= {0,0,0,0};        
   int a = 0;
   
   Serial.print("vetor :   ");
   for(int i=0; i < 4; i++){ //percorre as ações do estado colocando num vetor
   vetor[i] = Q[s][i]; // atribui a linha referente ao estado s 
        
        Serial.print("----");  
        Serial.print(vetor[i]); 
   } 
   Serial.println("");   
   
   int iMaior = 0;
   int indiceAnterior=0;
   int i = 0; 
   do {
        if (vetor[i] > vetor[indiceAnterior]) {
            iMaior = i;
            indiceAnterior = i;//in
        }     
        i++;
    } while (i < 4);
    
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
     int infra = analogRead(A0); 
  
   if( infra < 10){
    Serial.println("achou");
    acendeLed(1);
    return 1; // retorna que alcançou o objetivo 
   }else if(ultrassom <= 10){
    acendeLed(4);
    Serial.println("dentroHorta");   
    return 3;
   }else if(ultrassom > 10 && ultrassom <= 15){
    acendeLed(2);
    Serial.println("ancorado");   
    return 2; 
   }else if(ultrassom > 15 && ultrassom <= 20){
    acendeLed(3);
    Serial.println("foraHorta");   
    return 4;
   }else if(ultrassom > 20 ){ 
    acendeLed(0);
    Serial.println("perdido");  
    if(infra >=10 && infra < 20){
        return 5;
      }else if(infra >=20 && infra < 30){
      return 6;
      }else if(infra >=30 && infra < 40){
      return 7;
      } else if(infra >=40 && infra < 50){
      return 8;
      } else if(infra >=50 && infra < 60){
      return 9;
      } else if(infra >=60 && infra < 70){
      return 10;
      } else if(infra >=70 && infra < 80){
      return 11;
      } else if(infra >=80 && infra < 90){
      return 12;
      } else if(infra >=90 && infra < 100){
      return 13;
      } else if(infra >=100){
      return 14;
      }  
    //return 0;
   }  
    
 }
 


void exibeValores(int s, int a){
  
   Serial.println("-------------------------");
    Serial.print("luz :  ");
    Serial.print(analogRead(A0)); 
    Serial.print("    ultrassom : ");
    Serial.println(ultrasonic.Ranging(CM)); 
    Serial.print("estado : ");
    Serial.print(s);
    Serial.print("    acao : ");
    Serial.println(a);  
  //exibe Q
   for(int i=0;i < 15;i++){
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
void giraEsquerda(int vel){
  digitalWrite(trasDireito,LOW);//desativa os opostos
  digitalWrite(trasEsquerdo,LOW);
  analogWrite(frenteDireito,vel);  
  analogWrite(frenteEsquerdo,vel);  
  }
void giraDireita(int vel){  
  digitalWrite(frenteDireito,LOW);
  digitalWrite(frenteEsquerdo,LOW);
  analogWrite(trasDireito,vel); 
  analogWrite(trasEsquerdo,vel); 
  }
 void frente(int vel){
  digitalWrite(trasDireito,LOW);
  digitalWrite(frenteEsquerdo,LOW);
  analogWrite(frenteDireito,vel); 
  analogWrite(trasEsquerdo,vel);
  } 
  void tras(int vel){
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

  
  

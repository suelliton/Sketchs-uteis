#include <IRremote.h>  
  
int RECV_PIN = 3;  
float armazenavalor;  
int IN1 = 7;
int IN2 = 10;
int ENABLE1 = 8;
int IN3 = 6;
int IN4 = 9;
int ENABLE2 = 5;


   
IRrecv irrecv(RECV_PIN);  
decode_results results; 
int cont=0;  
void setup()  
{  
  pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(ENABLE1,OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 pinMode(ENABLE2,OUTPUT);
  
  Serial.begin(9600);  
  irrecv.enableIRIn(); // Inicializa o receptor IR  
  digitalWrite(ENABLE1,HIGH);
  digitalWrite(ENABLE2,HIGH);
}  


void andarFrente(){
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW); 
 
  
  }
 void andarTras(){
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH); 
 
 } 
 void giraDireita(){
  digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH); 
 
  
  }
 void giraEsquerda(){
  digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW); 
 
  
  }
  void parar(){
  digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW); 
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, LOW);
 
    
    }

   
void loop(){  

 
  if (irrecv.decode(&results))  
  {  
    Serial.print("Valor lido : ");  
    Serial.println(results.value, HEX);  
    armazenavalor = (results.value);  
    if (armazenavalor == 0xB54A02FD){  
       andarFrente();
      
    }  
     if (armazenavalor == 0xB54A827D){  
       
        andarTras();
       
    }  
     if (armazenavalor == 0xB54A42BD){  
       
            giraEsquerda();
      
    }
     if (armazenavalor == 0xB54AC23D){       
       giraDireita();
      
    }
    if(armazenavalor == 0xB54AB04F){
      parar();
      }
       

     
  irrecv.resume(); //Le o próximo valor  
  }  
}  

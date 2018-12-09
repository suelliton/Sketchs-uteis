#include <Servo.h>
#include <IRremote.h>  
Servo myservo;
int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);  
decode_results results; 
float leitura;

int ligaL = 8;
int lf=7;
int lb=10;

int ligaR = 5;
int rf=6;
int rb=9;

int posicao = 80;
int velocidade = 255;


void setup() {  
  myservo.attach(4);
  Serial.begin(9600);  
  irrecv.enableIRIn();
  myservo.write(posicao);
}

void loop() {
 
  if (irrecv.decode(&results)){  
    Serial.print("Valor lido : ");  
    Serial.println(results.value, HEX);  
    leitura = (results.value);  
    if (leitura == 0xB54AC23D){  
        if(posicao < 110){
        posicao = posicao + 5;   
        }
        myservo.write(posicao);
    }  
    if (leitura == 0xB54A42BD){  
        if(posicao > 55 ){
        posicao = posicao - 5;   
        }
        myservo.write(posicao);
    }

    
     if (leitura == 0xB54A50AF){   
      if(velocidade < 255){       
        velocidade = velocidade + 5;  
      }      
    }
     if (leitura == 0xB54AD02F){          
      if(velocidade > 25){
        velocidade = velocidade - 5;      
      }  
    }

    
    if (leitura == 0xB54A02FD  ){  
        andarFrente(velocidade);       
    }    
     if (leitura == 0xB54A827D){  
        andarTras(velocidade);    
    }
    
    if(leitura == 0xB54AB04F){
     digitalWrite(ligaL,LOW);
  digitalWrite(ligaR,LOW);
  digitalWrite(lf,LOW);
  digitalWrite(rf,LOW);  
  digitalWrite(lb,LOW);
  digitalWrite(rb,LOW);
      
      }
    
     Serial.println(posicao); 
     
     Serial.println(velocidade);  

     
  irrecv.resume(); //Le o próximo valor  
  }  

}
void andarFrente(int velocidade){
  analogWrite(ligaL,velocidade-20);
  analogWrite(ligaR,velocidade);
  digitalWrite(lf,HIGH);
  digitalWrite(rf,HIGH);  
  digitalWrite(lb,LOW);
  digitalWrite(rb,LOW);
  }
void andarTras(int velocidade){
  analogWrite(ligaL,velocidade-20);
  analogWrite(ligaR,velocidade);
  digitalWrite(lf,LOW);
  digitalWrite(rf,LOW);  
  digitalWrite(lb,HIGH);
  digitalWrite(rb,HIGH);
  }
void parar(){
  analogWrite(ligaL,0);
  analogWrite(ligaR,0);
  digitalWrite(lf,LOW);
  digitalWrite(rf,LOW);  
  digitalWrite(lb,LOW);
  digitalWrite(rb,LOW);
  }

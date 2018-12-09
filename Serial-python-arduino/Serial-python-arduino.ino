int led = 13;
char leituraSerial;

int estadoLed = 0;
void setup(){
  estadoLed = 0;
  Serial.begin(9600);
}

void loop(){
     
  digitalWrite(led,estadoLed);
  
  if(Serial.available() > 0){
  leituraSerial = Serial.read();  
  
  if(leituraSerial == '1'){
    estadoLed = 1;    
  }else if(leituraSerial == '2'){
    estadoLed = 0;   
  }
  
  
}}

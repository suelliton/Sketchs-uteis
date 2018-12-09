
int led1 = 8;  
int led2 = 9;
int led3 = 10;
int led4 = 11;  
int led5 = 12;  
  
  
  
void setup(){  
  Serial.begin(9600);  
  pinMode(led1, OUTPUT);   
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);  
  pinMode(led4, OUTPUT);  
  pinMode(led5, OUTPUT);  

}  
 //79B8EA47  -- direito  
 //129E14EB  -- esquerdo  
 //129E14EB  -- cima  
 //129E14EB  -- baixo  
void loop(){  
  for(int i=10;i>1;i=i-2){
  acendeUm(i);
  
  }
   for(int i=1;i<10;i=i=i+2){
  acendeUm(i);
  
  }
}  

void acendeUm(int d){
  digitalWrite(led1, HIGH);
  delay(d);
  digitalWrite(led2, HIGH);
  delay(d);
  digitalWrite(led3, HIGH);
  delay(d);
  digitalWrite(led4, HIGH);
  delay(d);
  digitalWrite(led5, HIGH);
  delay(d);
  digitalWrite(led1, LOW);
  delay(d);
  digitalWrite(led2, LOW);
  delay(d);
  digitalWrite(led3, LOW);
  delay(d);
  digitalWrite(led4, LOW);
  delay(d);
  digitalWrite(led5, LOW);
  delay(d);

  digitalWrite(led5, HIGH);
  delay(d);
  digitalWrite(led4, HIGH);
  delay(d);
  digitalWrite(led3, HIGH);
  delay(d);
  digitalWrite(led2, HIGH);
  delay(d);
  digitalWrite(led1, HIGH);
  delay(d);
  digitalWrite(led5, LOW);
  delay(d);
  digitalWrite(led4, LOW);
  delay(d);
  digitalWrite(led3, LOW);
  delay(d);
  digitalWrite(led2, LOW);
  delay(d);
  digitalWrite(led1, LOW);
  delay(d);
}
void acendeDois(int d){
  d=d/2;
  for(int i=0;i<3;i++){
  digitalWrite(led1, HIGH);
  delay(d);
  digitalWrite(led2, LOW);
  delay(d);
  digitalWrite(led3, HIGH);
  delay(d);
  digitalWrite(led4, LOW);
  delay(d);
  digitalWrite(led5, HIGH);
  delay(d);
  digitalWrite(led1, LOW);
  delay(d);
  digitalWrite(led2, HIGH);
  delay(d);
  digitalWrite(led3, LOW);
  delay(d);
  digitalWrite(led4, HIGH);
  delay(d);
  digitalWrite(led5, LOW);
  delay(d);
}
  
}

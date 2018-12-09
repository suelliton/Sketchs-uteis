
void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);

for(int i = 0 ; i < 5;i++){
    digitalWrite(3+i,HIGH)  ;
}
}

void loop() {
  // put your main code here, to run repeatedly:

}

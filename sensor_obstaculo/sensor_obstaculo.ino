int emissor = 12;
int receptor = A0;
int leitura;
void setup() {
  // put your setup code here, to run once:
pinMode(emissor,OUTPUT);
Serial.begin(9600);
}

void loop() {
  digitalWrite(emissor,HIGH);
  // put your main code here, to run repeatedly:
  leitura = analogRead(receptor);
  leitura = map(leitura,0,1023,0,255);
  Serial.println(leitura);
  delay(1000);
}

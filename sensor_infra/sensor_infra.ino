int sensor = A0;
int leitura;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
leitura = analogRead(sensor);
Serial.println(leitura);
delay(200);
}

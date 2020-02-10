int Entrada=37;
int ledPin = 13;
int Valor=0;
void setup()
{
  // put your setup code here, to run once:
  pinMode(Entrada,INPUT);
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);
   digitalWrite(Entrada, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Valor=digitalRead(Entrada);
  Serial.println(Valor);
  if(Valor==1)
    digitalWrite(ledPin, HIGH);
  else
     digitalWrite(ledPin, LOW);
  delay(100);
}

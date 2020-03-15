#include <LedControl.h>
#include <Stepper.h>

//Stepper
int Frecuencia = 0;
const int stepsPerRevolution = 300;
Stepper myStepper = Stepper(stepsPerRevolution, 52, 48, 50, 46);

//Motores
const int motor1 = 47;
const int motor1a = 49;

const int motor2 = 51;
const int motor2a = 53;

const int motor3 = 44;
const int motor3a = 45;

//Matriz
const int DIN = 2;
const int CLK = 3;
const int CS = 4;
LedControl lc = LedControl(DIN, CLK, CS, 1);

//Control de posicion
int posicionX = 0;
int posicionY = 0;

//Botones movimiento
const int BTNU = 5;
const int BTND = 6;
const int BTNL = 7;
const int BTNR = 8;
const int BTNB = 9;


int PENDIENTE = 0;

void setup() {
  pinMode(motor1,OUTPUT);
  pinMode(motor1a,OUTPUT); 
  pinMode(motor2,OUTPUT); 
  pinMode(motor2a,OUTPUT); 
  pinMode(motor3,OUTPUT); 
  pinMode(motor3a,OUTPUT); 
  digitalWrite(motor3,LOW);
  digitalWrite(motor3a,LOW);

  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  lc.clearDisplay(0);
  lc.setLed(0,0,0,true);

  pinMode(BTNU, INPUT);
  pinMode(BTND, INPUT);
  pinMode(BTNL, INPUT);
  pinMode(BTNR, INPUT);
  pinMode(BTNB, INPUT);
  myStepper.setSpeed(100);

  Serial.begin(9600);
}

void parpadear(int cantidad){
  for(int i = 0; i < cantidad; i++){
    lc.setLed(0, posicionY, posicionX, HIGH);
    delay(100);
    lc.setLed(0, posicionY, posicionX, LOW);
    delay(100);
  }
}

//True -> subir
//False -> bajar
void moverZ(bool tipo){
  if(tipo){
    digitalWrite(motor3,HIGH);
    digitalWrite(motor3a,LOW);
    parpadear(20);
    digitalWrite(motor3,LOW);
    digitalWrite(motor3a,LOW);
  }else{
    digitalWrite(motor3,LOW);
    digitalWrite(motor3a,HIGH);
    parpadear(15);
    digitalWrite(motor3,LOW);
    digitalWrite(motor3a,LOW);
  }
}

//True -> xmas
//False -> xmenos
void moverX(bool tipo){
  if(tipo){
    if(posicionX != 7){
      digitalWrite(motor1,HIGH);
      digitalWrite(motor1a,LOW);
      delay(585);
      digitalWrite(motor1,LOW);
      digitalWrite(motor1a,LOW);
      posicionX++;
    }
  }else{
    if(posicionX != 0){
      digitalWrite(motor1,LOW);
      digitalWrite(motor1a,HIGH);
      delay(600);
      digitalWrite(motor1,LOW);
      digitalWrite(motor1a,LOW);
      posicionX--;
    }
  }
  lc.setLed(0, posicionY, posicionX, true);
}

//True -> ymas
//False -> ymenos
void moverY(bool tipo){
  if(tipo){
    if(posicionY != 7){
      digitalWrite(motor2,HIGH);
      digitalWrite(motor2a,LOW);
      delay(375);
      digitalWrite(motor2,LOW);
      digitalWrite(motor2a,LOW);
      posicionY++;
    }
  }else{
    if(posicionY != 0){
      digitalWrite(motor2,LOW);
      digitalWrite(motor2a,HIGH);
      delay(375);
      digitalWrite(motor2,LOW);
      digitalWrite(motor2a,LOW);
      posicionY--;
    }
  }
  lc.setLed(0, posicionX, posicionY, true);
}

void dibujarFeliz(){
  lc.clearDisplay(0);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 4, true);
  lc.setLed(0, 4, 1, true);
  lc.setLed(0, 4, 3, true);
  lc.setLed(0, 4, 5, true);
  lc.setLed(0, 3, 1, true);
  lc.setLed(0, 3, 5, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 2, 3, true);
  lc.setLed(0, 2, 4, true);
}

void dibujarTriste(){
  lc.clearDisplay(0);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 4, true);
  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 4, 3, true);
  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 0, 1, true);
  lc.setLed(0, 0, 5, true);
  lc.setLed(0, 2, 2, true);
  lc.setLed(0, 2, 3, true);
  lc.setLed(0, 2, 4, true);
}

void loop() {
  if(digitalRead(BTNU) == HIGH){
    Serial.println("Y+");
    moverY(true);
  }
  if(digitalRead(BTND) == HIGH){
    Serial.println("Y-");
    moverY(false);
  }
  if(digitalRead(BTNR) == HIGH){
    Serial.println("X+");
    moverX(true);
  }
  if(digitalRead(BTNL) == HIGH){
    Serial.println("X-");
    moverX(false);
  }
  if(digitalRead(BTNB) == HIGH){
    Serial.println("Z-");
    moverZ(true);
    garra();
    Serial.println("Z+");
    moverZ(false);
  }
}

void garra(){
  //Abrir
  for(int i=0;i<20;i++)
  {
    //delay(100);
    myStepper.step(-stepsPerRevolution);
    Serial.println("Abriendo");
  }
  delay(3000);
  
  //Cerrar 
  for(int i=0;i<20;i++)
  {
    //delay(100);
    myStepper.step(stepsPerRevolution);
    Serial.println("Cerrando");
  }
}

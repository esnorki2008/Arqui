#include "LedControl.h"

//METODOS AGREGADOS MoverAlInicio   JuegoNuevo
#include <Stepper.h>
#include <LiquidCrystal.h>
//Veces Apachada La Garra
int VecesGarra=0;
//Buzzer
const int buzzer = 10;

//Sensor caja
const int caja = 6;

//Sensor Garra
const int sensorGarra = 7;

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

int prueba = 0;

//Botones movimiento
const int BTNU = 5;
const int BTND = 6;
const int BTNL = 7;
const int BTNR = 8;
const int BTNB = 9;

int PENDIENTE = 0;

//
int sensorReading = 0; //Pin análogo en espera
//Pines Analogos De Lectura
int MonedaQuetzal = 10;
int MonedaVeinti = 9;
int MonedaCincuenta = 11;
int DelayMonedas = 300;
//Variables Del Contador
bool RecibirMoneda=false;
int Credito = 500;   //Credito Actual
int UltimaMoneda = 0; //25,50,100
int CantidadMonedas100 = 0;
int CantidadMonedas50 = 0;
int CantidadMonedas25 = 0;
//Pantalla LCD
int RS = 22;
int Enable = 24;
int D4 = 26;
int D5 = 28;
int D6 = 30;
int D7 = 32;
//Modalidad Bluetooth
//Variables
int ModoDeJuego = 0;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);
//
void setup()
{
  pinMode(motor1, OUTPUT);
  pinMode(motor1a, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor3a, OUTPUT);
  digitalWrite(motor3, LOW);
  digitalWrite(motor3a, LOW);

  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  lc.clearDisplay(0);
  lc.setLed(0, 0, 0, HIGH);

  pinMode(BTNU, INPUT);
  pinMode(BTND, INPUT);
  pinMode(BTNL, INPUT);
  pinMode(BTNR, INPUT);
  pinMode(BTNB, INPUT);

  pinMode(buzzer, OUTPUT);

  myStepper.setSpeed(100);
  //Bluetooth Y Contador
  Serial2.begin(9600);
  Serial.begin(9600);

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Credito Actual ");
  JuegoNuevo();
}
void JuegoNuevo(){
  VecesGarra=0;
  posicionX = 0;
  posicionY = 0;  
  lc.clearDisplay(0);
  lc.setLed(0, posicionY, posicionX, HIGH);
}
void parpadear()
{
  for (int i = 0; i < 15; i++)
  {
    lc.setLed(0, posicionY, posicionX, HIGH);
    delay(100);
    lc.setLed(0, posicionY, posicionX, LOW);
    delay(100);
  }
}

void parpadear2()
{
  for (int i = 0; i < 20; i++)
  {
    lc.setLed(0, posicionY, posicionX, HIGH);
    delay(100);
    lc.setLed(0, posicionY, posicionX, LOW);
    delay(100);
  }
}

//True -> subir
//False -> bajar
void moverZ(bool tipo)
{
  
  if (tipo)
  {
    digitalWrite(motor3, HIGH);
    digitalWrite(motor3a, LOW);
    parpadear2();
    lc.setLed(0, posicionY, posicionX, HIGH);
    digitalWrite(motor3, LOW);
    digitalWrite(motor3a, LOW);
  }
  else
  {
    digitalWrite(motor3, LOW);
    digitalWrite(motor3a, HIGH);
    parpadear();
    lc.setLed(0, posicionY, posicionX, HIGH);
    digitalWrite(motor3, LOW);
    digitalWrite(motor3a, LOW);
  }
}

//True -> xmas
//False -> xmenos
void moverX(bool tipo)
{
  if (tipo)
  {
    if (posicionX != 7)
    {
      digitalWrite(motor1, HIGH);
      digitalWrite(motor1a, LOW);
      delay(585);
      digitalWrite(motor1, LOW);
      digitalWrite(motor1a, LOW);
      posicionX++;
    }
  }
  else
  {
    if (posicionX != 0)
    {
      digitalWrite(motor1, LOW);
      digitalWrite(motor1a, HIGH);
      delay(600);
      digitalWrite(motor1, LOW);
      digitalWrite(motor1a, LOW);
      posicionX--;
    }
  }
  lc.setLed(0, posicionY, posicionX, true);
}

//True -> ymas
//False -> ymenos
void moverY(bool tipo)
{
  if (tipo)
  {
    if (posicionY != 7)
    {
      digitalWrite(motor2, HIGH);
      digitalWrite(motor2a, LOW);
      delay(375);
      digitalWrite(motor2, LOW);
      digitalWrite(motor2a, LOW);
      posicionY++;
    }
  }
  else
  {
    if (posicionY != 0)
    {
      digitalWrite(motor2, LOW);
      digitalWrite(motor2a, HIGH);
      delay(375);
      digitalWrite(motor2, LOW);
      digitalWrite(motor2a, LOW);
      posicionY--;
    }
  }
  lc.setLed(0, posicionY, posicionX, true);
}

void dibujarFeliz()
{
  lc.clearDisplay(0);
  lc.setLed(0, 6, 2, HIGH);
  lc.setLed(0, 6, 4, HIGH);
  lc.setLed(0, 4, 1, HIGH);
  lc.setLed(0, 4, 5, HIGH);
  lc.setLed(0, 3, 1, HIGH);
  lc.setLed(0, 3, 5, HIGH);
  lc.setLed(0, 2, 2, HIGH);
  lc.setLed(0, 2, 3, HIGH);
  lc.setLed(0, 2, 4, HIGH);
}

void dibujarTriste()
{
  lc.clearDisplay(0);
  lc.setLed(0, 6, 2, HIGH);
  lc.setLed(0, 6, 4, HIGH);
  lc.setLed(0, 2, 1, HIGH);
  lc.setLed(0, 2, 5, HIGH);
  lc.setLed(0, 3, 1, HIGH);
  lc.setLed(0, 3, 5, HIGH);
  lc.setLed(0, 4, 2, HIGH);
  lc.setLed(0, 4, 3, true);
  lc.setLed(0, 4, 4, true);
}

void loop()
{
  AccionesDelContador();
  //Bluetooth
  if (Serial2.available())
  {
    RecibirMoneda=false;
    OperacionesBluetooth();
  }
  //Contador De Acciones De La Garra
  if(VecesGarra>=2){
    ModoDeJuego=0;
    VecesGarra=0;
  if (analogRead(caja) < 10)
    {
      
      Serial.println("GANO");
      dibujarFeliz();
      Serial2.print("l");
      Serial2.print("$");
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
    }
    else
    {
      
      Serial2.print("w");
      Serial2.print("$");
      Serial.println("PERDIO");
      dibujarTriste();
    }
 }
  //ModoDeJuego 1 Es Manual
  if (ModoDeJuego == 1)
  {
    if (digitalRead(BTND) == HIGH)
    {
      Serial.println("Y+");
      moverY(true);
    }
    if (digitalRead(BTNU) == HIGH)
    {
      Serial.println("Y-");
      moverY(false);
    }
    if (digitalRead(BTNR) == HIGH)
    {
      Serial.println("X+");
      moverX(true);
    }
    if (digitalRead(BTNL) == HIGH)
    {
      Serial.println("X-");
      moverX(false);
    }
    if (digitalRead(BTNB) == HIGH)
    {
      VecesGarra++;
      
      for (int i = 0; i < 20; i++)
      {
        //delay(100);
        myStepper.step(-stepsPerRevolution);
        Serial.println("Abriendo");
      }

      if (analogRead(sensorGarra) < 900)
      {
        Serial.println("Tiene");        
      }
      else
      {
        
        Serial.println("No Tiene");
        //Mover Al Inicio
        MoverAlInicio();
        if(VecesGarra==1)
          dibujarTriste();
      }

      Serial.println("Z-");
      moverZ(true);
      for (int i = 0; i < 20; i++)
      {
        //delay(100);
        myStepper.step(stepsPerRevolution);
        Serial.println("Cerrando");
      }
      Serial.println("Z+");
      moverZ(false);
    }
  }
}
void MoverAlInicio(){
  MoverAlInicio();
  for(int i=0;i<posicionX;i++){
    moverX(false);
  }
  for(int j=0;j<posicionY;j++){
    moverY(false);
  }
}
void garra()
{
  //Abrir

  delay(3000);

  //Cerrar
}

//Para El Contador
void AccionesDelContador()
{
  sensorReading = analogRead(MonedaQuetzal); //Instrucción para obtener dato analógico
  if (sensorReading < 280)
  {
    Serial.print("Uno   ");
    Serial.println(sensorReading);
    Credito = Credito + 100;
    delay(DelayMonedas);
  }

  sensorReading = analogRead(MonedaCincuenta); //Instrucción para obtener dato analógico
  if (sensorReading < 280)
  {
    Serial.print("Cincuenta   ");
    Serial.println(sensorReading);
    Credito = Credito + 50;
    delay(DelayMonedas);
  }
  sensorReading = analogRead(MonedaVeinti); //Instrucción para obtener dato analógico
  if (sensorReading < 360)
  {
    Serial.print("Veinticinco   ");
    Serial.println(sensorReading);
    Credito = Credito + 25;
    delay(DelayMonedas);
  }

  lcd.setCursor(0, 1);
  lcd.print(Credito);
}
//Operaciones Bluetooth
bool ModoRemoto(char m)
{
  if (ModoDeJuego != 2) //No Esta En Modo Remoto
    return true;

  if (m == 'u')
  { //MoverArriba
    Serial.println("MoviendoArriba");
    moverY(true);
    return true;
  }
  else if (m == 'd')
  { //MoverAbajo
    Serial.println("MoviendoAbajo");
    moverY(false);
    return true;
  }
  else if (m == 'l')
  { //MoverIzquierda
    moverX(false);
    Serial.println("MoviendoIzquierda");
    return true;
  }
  else if (m == 'r')
  { //MoverDerecha
    //MoverDerecha();
    Serial.println("MoviendoDerecha");
    moverX(true);
    return true;
  }
  else if (m == 'g')
  { //Agarrar
    VecesGarra++;
    for (int i = 0; i < 20; i++)
    {
      //delay(100);
      myStepper.step(-stepsPerRevolution);
      Serial.println("Abriendo");
    }

    if (analogRead(sensorGarra) < 900)
    {
      Serial.println("Tiene");
    }
    else
    {
      Serial.println("No Tiene");

      MoverAlInicio();
        if(VecesGarra==1)
          dibujarTriste();
    }

    Serial.println("Z-");
    moverZ(true);
    for (int i = 0; i < 20; i++)
    {
      //delay(100);
      myStepper.step(stepsPerRevolution);
      Serial.println("Cerrando");
    }
    Serial.println("Z+");
    moverZ(false);
    return true;
  }
  else if (m == 's')
  { //Soltar
    VecesGarra++;
    for (int i = 0; i < 20; i++)
    {
      //delay(100);
      myStepper.step(-stepsPerRevolution);
      Serial.println("Abriendo");
    }

    if (analogRead(sensorGarra) < 900)
    {
      Serial.println("Tiene");
    }
    else
    {
      Serial.println("No Tiene");

      MoverAlInicio();
        if(VecesGarra==1)
          dibujarTriste();
    }

    Serial.println("Z-");
    moverZ(true);
    for (int i = 0; i < 20; i++)
    {
      //delay(100);
      myStepper.step(stepsPerRevolution);
      Serial.println("Cerrando");
    }
    Serial.println("Z+");
    moverZ(false);
    return true;
  }
  return false;
}
void OperacionesBluetooth()
{
  bool Continuar = true;
  //Serial2.print(450);
  char m = Serial2.read();
  Serial.println(m);
  if (m == 'y')
  { //Desactivar Modos
    ModoDeJuego = 0;
  }
  else if (m == 'x')
  { //ModoManual
    JuegoNuevo();    
    ModoDeJuego = 1;
  }
  else if (m == 'z')
  { //ModoRemoto
    JuegoNuevo();    
    ModoDeJuego = 2;
  }
  else if (m == 'i')
  { //Enviar Info Monedas
    if (Credito < 0)
      Credito = 0;

    RecibirMoneda=true;
    Serial2.print(Credito);
    Serial2.print(',');
    Serial2.print(CantidadMonedas100);
    Serial2.print(',');
    Serial2.print(CantidadMonedas50);
    Serial2.print(',');
    Serial2.print(CantidadMonedas25);
    Serial2.print(',');
    Serial2.print(UltimaMoneda);
    Serial2.print('$');
    delay(1000);
  }
  else if (m == 'c')
  { //Comprar Boleto
    Credito = Credito - 100;
  }
  else
  {
    //delay(1000);//Delay Para Que No Duplique Informacion
    if (!ModoRemoto(m))
    {
      Serial.println("Desconocido"); //Bluetooth Desconocido
      Serial.println(m);
    }
    /*char hola[2];
        hola[0]='n';
        hola[1]='o';
        */
  }
}

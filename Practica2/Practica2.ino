#include <LiquidCrystal.h>
#include <Stepper.h>
#include "Simbolos.h"
int Frecuencia=0;

const int stepsPerRevolution = 300;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);
int NumProductos=100;
//int S1,S0,S2,S3,Out;
int trigger, eco, duracion, distancia;
int peq, med, gra;

int tam = 0;
int color = 0;

int cRojo = 0;
int cAzul = 0;
int cVerde = 0;

bool pasoColor = false;
bool pasoTam = false;
bool salioTam = false;
bool calcularDireccion = false;


#define S0 4
#define S1 3
#define S2 6
#define S3 7
#define Out 5
void setup() {
  myStepper.setSpeed(5);
  //Control de UltraSonico
  peq = 5;
  med = 4;
  gra = 2;
  trigger = 24;
  eco = 22;
  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);
  //Iniciar Color
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(Out,INPUT);
  //Iniciar Pantalla
  Serial.begin(9600); 
  lcd.begin(16, 2);
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
}

void loop() {
  CadenaFuncionamiento();
  ComprobarColor();
  ComprobarDistancia();
  if(calcularDireccion){
    CalcularDireccion();
  }
}

void CalcularDireccion(){
  Serial.print("Color: ");
  switch(color){
    case 1:
      Serial.print("Rojo");
      break;
    case 2:
      Serial.print("Verde");
      break;
    case 3:
      Serial.print("Azul");
      break;
  }
  Serial.println("  ");
  Serial.print("Tamaño: ");
  switch(tam){
    case 1:
      Serial.print("Pequeña");
      break;
    case 2:
      Serial.print("Mediana");
      break;
    case 3:
      Serial.print("Grande");
      break;
  }
  Serial.println("   ");

  pasoColor = false;
  pasoTam = false;
  tam = 0;
  color = 0;
  calcularDireccion = false;
}

void ComprobarDistancia(){
  digitalWrite(trigger, HIGH);
  delay(1);
  digitalWrite(trigger, LOW);
  
  duracion = pulseIn(eco, HIGH);
  distancia = duracion / 58.2;
  if(distancia < 0 || distancia > 6){
    distancia = 6;
  }

  //Pequeña = 1; Mediana = 2; Grande = 3
  if(distancia == peq){
    if(tam == 0)
      tam = 1;
    pasoTam = true;
    //Serial.println("Pequeña");
  } else if(distancia == med){
    if(tam < 2)
      tam = 2;
    pasoTam = true;
    //Serial.println("Mediana");
  } else if(distancia == gra ){
    if(tam < 3)
      tam = 3;
    pasoTam = true;
    //Serial.println("Grande");
  }else{
    if(pasoTam == true && pasoColor == true){
      calcularDireccion = true;
    }
   }
}


void ComprobarColor(){

  int Delay=100;
  
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  
  Frecuencia=pulseIn(Out,LOW);
  cRojo =map(Frecuencia,2250,650,0,255);
  delay(Delay);

  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  cVerde =map(Frecuencia,2250,650,0,255);
  delay(Delay);


  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  cAzul =map(Frecuencia,2250,650,0,255);

  //Rojo = 1; Verde = 2; Azul = 3
  if(cVerde < 0 && cRojo < 0){
    
  } else {
    if(cVerde > cRojo && cVerde > 30 && cAzul < 100){
      color = 2;
    }else if(cRojo > cAzul && cRojo > cVerde){
      color = 1;
    } else if(cAzul > cRojo && cAzul > cVerde && cRojo < 40){
      if(color == 0){
        color = 3;
      }
    } else if(cVerde > cRojo){
      color = 2;
    }
    pasoColor = true;
  }
  
  delay(Delay);
  
}

//Cadena Mientras Estan En Funcionamiento
void CadenaFuncionamiento(){
  lcd.setCursor(0,0);
  lcd.print("G16 - Total");
  lcd.setCursor(0,1);
  lcd.print(ProductosTotales);
  lcd.setCursor(3,1);
  lcd.print("Productos-");
  lcd.createChar(0,CaraFeliz);
  lcd.setCursor(15,1);
  lcd.write((byte)0);

}
void CadenaFinalRecipiente1(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.setCursor(0,0);
  lcd.createChar(1,DoblePunto);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16–Recipiente");
  lcd.setCursor(0,1);
  lcd.print("1");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente1);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)1);
}

void CadenaFinalRecipiente2(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.setCursor(0,0);
  lcd.createChar(1,DoblePunto);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16–Recipiente");
  lcd.setCursor(0,1);
  lcd.print("2");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente2);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)1);
}

void CadenaFinalRecipiente3(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.setCursor(0,0);
  lcd.createChar(1,DoblePunto);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16–Recipiente");
  lcd.setCursor(0,1);
  lcd.print("3");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente3);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)1);
}

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

int size = 0;
int color = 0;

int cRojo = 0;
int cAzul = 0;
int cVerde = 0;

bool pasoColor = false;
bool calcularColor = false;

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
  
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(Out,INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);
  
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(Out,INPUT);
  Serial.begin(9600); 
  lcd.begin(16, 2);
  

 
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
}

void loop() {
  CadenaFuncionamiento();
  ComprobarColor();
  ComprobarDistancia();
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
  if(distancia == peq){
    Serial.println("Caja Pequeña");
  } else if(distancia == med){
    Serial.println("Caja Mediana");
  } else if(distancia == gra ){
    Serial.println("Caja Grande");
  }else{
    Serial.println(distancia); 
   }
}


void ComprobarColor(){

  int Delay=100;
  
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  
  Frecuencia=pulseIn(Out,LOW);
  cRojo =map(Frecuencia,2250,650,0,255);
  /*Serial.print("R=");
  Serial.print(cRojo);  
  Serial.print("    ");*/
  delay(Delay);

  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  cVerde =map(Frecuencia,2250,650,0,255);
  /*Serial.print("G=");
  Serial.print(cVerde);  
  Serial.print("    ");*/
  delay(Delay);


  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  cAzul =map(Frecuencia,2250,650,0,255);
  /*Serial.print("B=");
  Serial.print(cAzul);*/  

  Serial.print("Rojo: ");
  Serial.print(cRojo);
  Serial.print(" Verde: ");
  Serial.print(cVerde);
  Serial.print(" Azul: ");
  Serial.print(cAzul);
  Serial.println("  ");

  //Rojo = 1; Verde = 2; Azul = 3
  if(cVerde < 0 && cRojo < 0){
    Serial.println("Nada");
    if(pasoColor == true){
      calcularColor = true;
      color = 0;
    }
  } else {
    if(cVerde > cRojo && cVerde > 30 && cAzul < 100){
      color = 2;
      Serial.println("Verde");
    }else if(cRojo > cAzul && cRojo > cVerde){
      if(color == 0){
        color = 1;
      }
      Serial.println("Rojo");
    } else if(cAzul > cRojo && cAzul > cVerde && cRojo < 40){
      if(color == 0){
        color = 3;
      }
      Serial.println("Azul");
    } else if(cVerde > cRojo){
      color = 2;
      Serial.println("Verde");
    }
    pasoColor = true;
  }
  
  
  Serial.println("");
  delay(Delay);
  
}

//Cadena Mientras Estan En Funcionamiento
void CadenaFuncionamiento(){
  lcd.setCursor(0,0);
  lcd.print("G16 - Total");
  lcd.setCursor(0,1);
  lcd.print("Productos-");
  lcd.setCursor(11,1);
  lcd.print(NumProductos);
  lcd.createChar(0,CaraFeliz);
  lcd.setCursor(15,1);
  lcd.write((byte)0);
  /*lcd.createChar(0,CaraFeliz);
  lcd.setCursor(0,1);
  lcd.write((byte)0);
  lcd.createChar(1,Amperson);
  lcd.setCursor(1,1);
  lcd.write((byte)1);*/
}

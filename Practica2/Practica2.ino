#include <LiquidCrystal.h>
#include "Simbolos.h"
int Frecuencia=0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int NumProductos=100;
int S1,S0,S2,S3,Out;
int trigger, eco, duracion, distancia;
int peq, med, gra;

void setup() {
  S1=30;
  S0=32;
  S3=34;
  S2=36;
  Out=38;
  
  //Control de UltraSonico
  peq = 7;
  med = 6;
  gra = 5;
  trigger = 10;
  eco = 9;
  
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(Out,INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);
  
  Serial.begin(9600); 
  lcd.begin(16, 2);


 
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
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
  
  if(DISTANCIA == peq){
    Serial.println("Caja Pequeña");
  } else if(DISTANCIA == med){
    Serial.println("Caja Mediana");
  } else if(DISTANCIA == gra){
    Serial.println("Caja Grande");
  }
  
}


void ComprobarColor(){

  
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  Frecuencia=pulseIn(Out,LOW);
  Frecuencia =map(Frecuencia,350,20,0,255);
  Serial.print("R=");
  Serial.print(Frecuencia);  
  Serial.print("    ");
  delay(100);

  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  Frecuencia =map(Frecuencia,350,20,0,255);
  Serial.print("G=");
  Serial.print(Frecuencia);  
  Serial.print("    ");
  delay(100);


  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frecuencia=pulseIn(Out,LOW);
  Frecuencia =map(Frecuencia,350,20,0,255);
  Serial.print("B=");
  Serial.print(Frecuencia);  
  
  Serial.println("");
  delay(100);
  
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

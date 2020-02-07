#include "FrequencyTimer2.h"
#include "Simbolos.cpp"
#include "LedControl.h"

LedControl lc=LedControl(10,11,12,1); 
byte col = 0;
byte leds[8][8];
byte leds2[8][8];
byte Abajo[8][8];
int Veces=0;
int Desp=9;
int ModoPantalla=0;// 0 Nombre    1 Juego
int pins[17]= {-1, 2, 3, 4, 5, 6, 7, 8, 9, 22, 24, 26,28,30, 32, 34,36};

int rows[8] = {pins[1], pins[2], pins[5], pins[4], pins[3], pins[6], pins[7], pins[8]};

int cols[8] = {pins[16], pins[15], pins[14], pins[13], pins[12], pins[11], pins[10], pins[9]}; 
 
const int TamanioMensaje =28;
int PosX,PosY,Letra;


byte Mensaje[TamanioMensaje][8][8] = {
 ESPACIO,G,UNO,TRES,ESPACIO,GUION,ESPACIO,S,E,C,C,I,O,N,ESPACIO,A,ESPACIO,GUION,ESPACIO,P,R,A,C,T,I,C,A,UNO
};
int pattern = 0;
void setup() {
  //ModoPantalla=0;
  PosX=PosY=Letra=0;

  // Configuramos los pinso como salida
  for (int i = 1; i <= 16; i++) {
    pinMode(pins[i], OUTPUT);
  }
 
  // inicializamos filas y columnas
  for (int i = 1; i <= 8; i++) {
    digitalWrite(cols[i - 1], LOW);
  }
 
  for (int i = 1; i <= 8; i++) {
    digitalWrite(rows[i - 1], LOW);
  }
 
  LimpiarLeds();
  EmpezarMatriz();
  FrequencyTimer2::disable();
  FrequencyTimer2::setPeriod(2000);
  FrequencyTimer2::setOnOverflow(display);
  PatronIncial(pattern);
}
 
void loop() {
  if(ModoPantalla==0){
    pattern = ++pattern % TamanioMensaje;
    DeslizarMatriz1(pattern, 100);
    GraficarMatriz2();
  }if(ModoPantalla==1){
    
  }
    
}


void GraficarMatriz2()
{
  for(int j=0;j<8;j++){
  for (int i = 0; i < 8; i++)  
  {
     if(leds2[i][j]==1)
      lc.setLed(0, i, j, true);
     else
     lc.setLed(0, i, j, false);
  }
  }
}





void EmpezarMatriz(){
  lc.shutdown(0,false); 
  lc.shutdown(1,false);
  lc.setIntensity(0,0);  
  lc.setIntensity(1,1);
  lc.clearDisplay(0); 
  lc.clearDisplay(1);  
} 
void LimpiarLeds() {
 
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = 0;
    }
  }
}
 
void PatronIncial(int Num) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = Mensaje[Num][i][j];
    }
  }
}
 void MoverAbajo(int pattern, int del) {
  PosY++;
  if(PosY>223){
    PosY=0;  
  }
    for(int z=0;z<8;z++){
    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        leds[j][i] = Mensaje[NumLetra(PosY+j)][NumY(PosY+j)][0 + i];
        leds2[j][i] = Mensaje[NumLetra(PosY+j+Desp)][NumY(PosY+j+Desp)][0 + i];    
      }     
    }  
    }
    delay(del);
}



void LimpiarMatrizAbajo(){
 
    for(int j=0;j<8;j++){
      for(int i=0;i<8;i++){
        Abajo[j][i]=0; 
       }  
    }
  
}
void MoverArriba(int pattern, int del) {
  PosY--;
  if(PosY<0){
    PosY=223;  
  }
    for(int z=0;z<8;z++){
    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        leds[7-j][i] = Mensaje[NumLetra(PosY-j)][NumY(PosY-j)][0 + i];
        leds2[7-j][i] = Mensaje[NumLetra(PosY-j+Desp)][NumY(PosY-j+Desp)][0 + i];     
      }     
    }  
    }
    delay(del);
    //CopiarAbajo();
}
void DeslizarMatriz1(int Patron, int Delay) {
  
  if(Veces<80){
  Veces++;
  MoverAbajo(Patron,Delay);
  }else if(Veces>81) {
  MoverArriba(Patron,Delay);
    
    Veces--;
  }else{
    MoverArriba(Patron,Delay);
      Veces=160;
      Veces--;
    }
}

void display() {
  digitalWrite(cols[col], LOW); 
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int row = 0; row < 8; row++) {
    if (leds[col][7 - row] == 1) {
      digitalWrite(rows[row], LOW);  
    }
    else {
      digitalWrite(rows[row], HIGH);
    }
  }
  digitalWrite(cols[col], HIGH);
}
int NumLetra(int Num){
  if(Num>223)
    return 0;
  else if(Num<0)
    return 27;  
  return Num/8;
}
int NumY(int Num){
  if(Num>223)
    return Num-223;
  else if(Num<=0)
    return 0;
       
  return Num%8;  
}

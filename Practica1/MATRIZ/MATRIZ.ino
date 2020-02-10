#include "FrequencyTimer2.h"
#include "Simbolos.cpp"
#include "LedControl.h"

LedControl lc=LedControl(10,11,12,1); 
byte col = 0;
byte Leds[8][8];
byte Leds2[8][8];
byte Abajo[8][8];

//Botones
int BtnPausa=37;
int BtnArriba=39;
int BtnAbajo=41;
//
byte MostrarMemoria[8][8];
int Veces=0;
int Desp=9;
//Informacion Para El Juego
int TiempoPausa=0;//Tiempo Presionado El De Pausa
boolean PresionadoAntes=false;//Para Saber Si Fue Presionado
boolean EstuvoPausado=false;
//
int FrecuenciaJuego=100;
//
byte Tablero[17][8];//Tablero De Juego
int PosJugador=4;//Posicion Del Jugador
//
int MiliSegundos=0;//Para Contar Segundos
int Segundos=0;//Tiempo Acumulado 
//
int PasosPoner=0;//Para Saber Si Se Poner Obstaculos
int PasosMover=0;//Para Saber Si Se Mueven Obstaculos
byte EstadoJuego=0; //0 NoIniciado  1 Empezado   2 Pausado   3 Finalizado
int ModoPantalla=0;// 0 Nombre    1 CuentaRegresiva   2 Juego    3 Pausa   -1 ProbarMatrices
//Pin Digitales Del Arduino    
int pins[17]= {-1, 2, 3, 4, 5, 6, 7, 8, 9, 22, 24, 26,28,30, 32, 34,36};

//Control de velocidad
int velocidad = 0;
//Control de movimiento
int movimiento = 0;

int rows[8] = {pins[1], pins[2], pins[5], pins[4], pins[3], pins[6], pins[7], pins[8]};

int cols[8] = {pins[16], pins[15], pins[14], pins[13], pins[12], pins[11], pins[10], pins[9]}; 
 
const int TamanioMensaje =28;
int PosX,PosY,Letra;

byte Digito[10][8][8]={CERO,UNO,DOS,TRES,CUATRO,CINCO,SEIS,SIETE,OCHO,NUEVE};
byte Mensaje[TamanioMensaje][8][8] = {
 ESPACIO,G,UNO,SEIS,ESPACIO,GUION,ESPACIO,S,E,C,C,I,O,N,ESPACIO,A,ESPACIO,GUION,ESPACIO,P,R,A,C,T,I,C,A,UNO
};
int pattern = 0;
void setup() {
   //Inputs
  pinMode(BtnPausa,INPUT);
  digitalWrite(BtnPausa, HIGH);
  pinMode(BtnArriba,INPUT);
  digitalWrite(BtnArriba, HIGH);
  pinMode(BtnAbajo,INPUT);
  digitalWrite(BtnAbajo, HIGH);
  
  Serial.begin(9600); 
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
  FrequencyTimer2::setPeriod(1000);
  FrequencyTimer2::setOnOverflow(display);
  PatronIncial(pattern);
}
 
void loop() {
  
  
  switch(ModoPantalla){
    case -1:
      ModoPrueba();
    break;
    case 0://Mostrar Informacion Grupo
      GrupoEnPantalla();
      break;
    case 1://Cuenta Regresiva Inicia Juego
      CuentaRegresiva();
      break;
    case 2:
      ContinuarJuego();
      break;
    case 3:
      Pausar_Fin();
      break;
    case 4:
      Pausar_Fin(); 
      break;
  
    
  }
    
}

void Pausar_Fin(){
  EstuvoPausado=true;
  if(Segundos>99)
    Segundos=99;
  //Separar Datos
  int Unidad=Segundos%10;
  int Decena=Segundos/10;
  //Cargar Datos
  NumToArreglo(Unidad);
  CargarADisplay(false);      
  NumToArreglo(Decena);
  CargarADisplay(true);
  GraficarMatriz2();
  MiliSegundos=0;
  int Inputs=digitalRead(BtnPausa);
    if(Inputs==0 && PresionadoAntes==false){
       if(ModoPantalla==3){
        ModoPantalla=1;
       }else{ 
          ModoPantalla=0;
       }
     }  
     else{
        PresionadoAntes=false;
        delay(1000);
     }

   
  delay(50);
}
void CuentaRegresiva(){
    
    int Seg=MiliSegundos/1000;
    if(Seg>3)
      Seg=3;
        
    NumToArreglo(3-Seg);  
    CargarADisplay(true);
    CargarADisplay(false);   
    GraficarMatriz2();
    MiliSegundos=MiliSegundos+1000;      
    delay(1000);
   
    
    if(Seg>=3){
       ModoPantalla=2;
       
       
       if(!EstuvoPausado){
          NuevoJuego();
          Segundos=0;
          MiliSegundos=0;
       }
       else
       EstuvoPausado=false;
    }
 }
void GrupoEnPantalla(){
  
    //Lee el dato del potenciometro
    velocidad = analogRead(0);
    //Mapea el dato del potenciometro
    velocidad = map(velocidad, 0, 1024, 0, 255);
    

    int Inputs=digitalRead(BtnArriba);
    if(Inputs==0)
        movimiento=0;

    Inputs=digitalRead(BtnAbajo);
    if(Inputs==0)
        movimiento=1;
    
    Inputs=digitalRead(BtnPausa);
    if(Inputs==0)
        ModoPantalla=1;
    
    //Envia el dato del potenciometro como velocidad hacia la funcion DeslizarMatriz1
    DeslizarMatriz1(pattern, velocidad, movimiento);
    GraficarMatriz2();
}
/*Area Juego*/
//Valor 0 Posicion De origen De Obstaculos
bool Colisiono(){
  if(PosJugador>7 || PosJugador<0)
    PosJugador=7;
  
  if(Tablero[15][PosJugador]==1 || PosJugador<=0 || PosJugador>=7)
    return true;
  else
    return false;
}
void NuevoJuego(){
  LimpiarMatrices();
  LimpiarJuego();
  MantenerBarreras();
  PosJugador=4;
  MiliSegundos=0;
  Segundos=0;
  EstadoJuego=1;
  PasosPoner=0;
  PasosMover=0;
}

int CalcularDificultad(int Seg){
   int Dificultad=10;

  switch(Seg/10){
    case 0:
      Dificultad=35;
    break;
    case 1:
      Dificultad=30;
    break; 
    case 2:
      Dificultad=25;
    break; 
    case 3:
      Dificultad=20;
    break;    
    case 4:
      Dificultad=15;
    break;  
    case 5:
      Dificultad=10;
    break;
    default:
      Dificultad=6;
    break;   
    
  }
  return Dificultad; 
}
void MantenerBarreras(){
  for(int i=0;i<16;i++){//Crear Barreras
    Tablero[i][0]=1;
    Tablero[i][7]=1;  
  } 
}
void LimpiarJuego(){
  for(int j=0;j<17;j++){
    for(int i=0;i<8;i++){
        Tablero[j][i]=0;
      
      }
    
  }  
}
void LimpiarMatrices(){
  for(int j=0;j<8;j++){
    for(int i=0;i<8;i++){
        Leds[i][j]=0;
        Leds2[i][j]=0;
      
      }
    
  }  
  
}

void ModoPrueba(){
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
        //Leds[i][j]=1;
        Leds[i][j]=Digito[1][i][j];
      } 
  }  
}

void ContinuarJuego(){//Mover Obstaculos
  MostrarJuego();
  GraficarMatriz2();
  PasosPoner++;
  PasosMover++;
  //Area Del Control
  int Inputs=digitalRead(BtnArriba);
    if(Inputs==0)
        PosJugador++;

    Inputs=digitalRead(BtnAbajo);
    if(Inputs==0)
        PosJugador--;
    
    Inputs=digitalRead(BtnPausa);
    if(Inputs==0){
      PresionadoAntes=true;
      if(PresionadoAntes)
        TiempoPausa=TiempoPausa+FrecuenciaJuego;
    }else{
        PresionadoAntes=false;
        TiempoPausa=0;  
    }

    if(TiempoPausa>=3000){
              ModoPantalla=3;//Ir A Pausa
            
    }
        
  //Fin Control
  ActualizarSegundos(MiliSegundos);
  int Seg=Segundos;
  int Dificultad=CalcularDificultad(Seg);
  MiliSegundos=MiliSegundos+FrecuenciaJuego;
if((Dificultad-5)-PasosMover<0){//Mover Obstaculos 
  for(int j=15;j>0;j--){   
    for(int i=1;i<7;i++){
        Tablero[j][i]=Tablero[j-1][i];
        Tablero[j-1][i]=0;    
    }  
}
 PasosMover=0;
}
if((Dificultad+4)-PasosPoner<0){//Poner Obstaculo  
  PasosPoner=0;
  CrearObstaculo();
}   
delay(FrecuenciaJuego);
  if(Colisiono()){
    NuevoJuego();
    ModoPantalla=1;//Terminar Juego  
    MiliSegundos=0;
    Segundos=0;
  }  
}
void MostrarJuego(){
  for(int i=0;i<8;i++){
    Leds2[7][i]=0;
  }
  
  for(int j=0;j<8;j++){
    for(int i=0;i<8;i++){    
        Leds[j][i]=Tablero[j][i];       
    }  
  }

  for(int j=0;j<8;j++){
    for(int i=0;i<8;i++){    
        Leds2[j][i]=Tablero[j+8][i];       
    }  
  }

    Leds2[7][PosJugador]=1;
}

void CrearObstaculo(){
  int PosInicial=random(0, 4);  
  for(int i=1;i<7;i++){
    if((PosInicial>=i && i<(PosInicial+3)))//Obstaculo Longitud 3
       Tablero[0][PosInicial+i]=1;  
  }
}
/*Fin Area Juego*/
void GraficarMatriz2()
{
  for(int j=0;j<8;j++){
  for (int i = 0; i < 8; i++)  
  {
     if(Leds2[i][j]==1)
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
      Leds[i][j] = 0;
    }
  }
}
 
void PatronIncial(int Num) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Leds[i][j] = Mensaje[Num][i][j];
    }
  }
}
//Actualiza Las Matrices Del Display Restando Posicion Actual
void MoverArriba(int pattern, int del) {
  PosY--;
  if(PosY<0){
    PosY=223;  
  }
    for(int z=0;z<8;z++){
    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        Leds[7-j][i] = Mensaje[NumLetra(PosY-j)][NumY(PosY-j)][0 + i];
        Leds2[7-j][i] = Mensaje[NumLetra(PosY-j+Desp)][NumY(PosY-j+Desp)][0 + i];     
      }     
    }  
    }
    delay(del);
}
 void MoverAbajo(int pattern, int del) {
  PosY++;
  if(PosY>223){
    PosY=0;  
  }
    for(int z=0;z<8;z++){
    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        Leds[j][i] = Mensaje[NumLetra(PosY+j)][NumY(PosY+j)][0 + i];
        Leds2[j][i] = Mensaje[NumLetra(PosY+j+Desp)][NumY(PosY+j+Desp)][0 + i];    
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

void DeslizarMatriz1(int Patron, int Delay, int Movimiento) {

    if(Movimiento == 0){
      //Se mueve hacia abajo
      MoverAbajo(Patron, Delay);
    } else {
      //Se mueve hacia arriba
      MoverArriba(Patron, Delay);      
    }
}
/*Escritura Display*/
void display() {
  
  
  digitalWrite(cols[col], LOW); 
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int row = 0; row < 8; row++) {
    if (Leds[col][7 - row] == 1) {
      digitalWrite(rows[row], LOW);  
    }
    else {
      digitalWrite(rows[row], HIGH);
    }
  }
  digitalWrite(cols[col], HIGH);
}

void ActualizarSegundos(int Milis){
  int Segu=Milis/1000;
  if(Segu>=1){
    Segundos++;
    MiliSegundos=0;
  }
}
void NumToArreglo(int Num){
  if(Num>9 || Num<0)
    Num=9;
  for(int j=0;j<8;j++){
    for(int i=0;i<8;i++){
        MostrarMemoria[j][i]=Digito[Num][j][i];
    }      
  }
    
}
void CargarADisplay(bool DisplayUno){
  for(int j=0;j<8;j++){
     for(int i=0;i<8;i++){
        if(DisplayUno){
          Leds[j][i]=MostrarMemoria[j][i]; 
        }else{
          Leds2[j][i]=MostrarMemoria[j][i]; 
        }
      }
  }
}
/**/
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

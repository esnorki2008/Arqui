#include <LiquidCrystal.h>
#include <Stepper.h>
#include "Simbolos.h"
int Frecuencia = 0;

const int stepsPerRevolution = 200;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);
int NumProductos = 100;
//int S1,S0,S2,S3,Out;
int trigger, eco, duracion, distancia;
int peq, med, gra;

int BtnDetener = 0;
//El Estado De La Banda
bool EstaFuncionando;
int tam = 0;
int color = 0;
int ProductosTotales;
int ProductosRecipiente1;
int ProductosRecipiente2;
int ProductosRecipiente3;

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

  ProductosTotales = 0;
  ProductosRecipiente1 = 0;
  ProductosRecipiente2 = 0;
  ProductosRecipiente3 = 0;
    EstaFuncionando=false;
  //Boton Entrada
  pinMode(BtnDetener,INPUT);
  digitalWrite(BtnDetener, HIGH);
  myStepper.setSpeed(35);

  //Control de UltraSonico
  peq = 5;
  med = 4;
  gra = 3;
  trigger = 24;
  eco = 22;

  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);

  //Iniciar Color
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(Out, INPUT);

  //Iniciar Pantalla
  Serial.begin(9600);
  lcd.begin(16, 2);



  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
}

void loop() {
  int Inputs=digitalRead(BtnDetener);
  if(Inputs==0){
    //Se Presiono El Boton
    if(EstaFuncionando){
      CadenaFinalRecipiente1();
      delay(60000);
      CadenaFinalRecipiente2();
      delay(60000);
      CadenaFinalRecipiente3();
      delay(60000);
      EstaFuncionando = false;
    } else {
      EstaFuncionando = true;
    }
  } else {
    CadenaFuncionamiento();
  if (color == 0) {
    ComprobarColor();
  }
  ComprobarDistancia();
  if (calcularDireccion) {
    CalcularDireccion();

    pasoColor = false;
    pasoTam = false;
    tam = 0;
    color = 0;
    calcularDireccion = false;
  }
  }
}

void MoverRecipiente(int numero) {
  if(ProductosTotales == 9){
    ProductosTotales = 0;
  } else {
    ProductosTotales++;
  }
  switch (numero) {
    case 1:
      myStepper.step(-stepsPerRevolution);
      delay(2000);
      myStepper.step(stepsPerRevolution);
      break;
    case 3:
      myStepper.step(stepsPerRevolution);
      delay(2000);
      myStepper.step(-stepsPerRevolution);
      break;
  }
}

void CalcularDireccion() {
  delay(1500);
  switch (color) {
    case 1:
      switch (tam) {
        case 1:
          //Rojo y Pequeña
          //Recipiente 2
          MoverRecipiente(2);
          break;
        case 2:
          //Rojo y Mediana
          //Recipiente 1
          MoverRecipiente(1);
          break;
        case 3:
          //Rojo y Grande
          //Recipiente 3
          MoverRecipiente(3);
          break;
      }
      break;
    case 2:
      switch (tam) {
        case 1:
          //Verde y Pequeña
          //Recipiente 3
          MoverRecipiente(3);
          break;
        case 2:
          //Verde y Mediana
          //Recipiente 2
          MoverRecipiente(2);
          break;
        case 3:
          //Verde y Grande
          //Recipiente 1
          MoverRecipiente(1);
          break;
      }
      break;
    case 3:
      switch (tam) {
        case 1:
          //Azul y Pequeña
          //Recipiente 1
          MoverRecipiente(1);
          break;
        case 2:
          //Azul y Mediana
          //Recipiente 3
          MoverRecipiente(3);
          break;
        case 3:
          //Azul y Grande
          //Recipiente 2
          MoverRecipiente(2);
          break;
      }
      break;
  }
}

void ComprobarDistancia() {
  digitalWrite(trigger, HIGH);
  delay(1);
  digitalWrite(trigger, LOW);

  duracion = pulseIn(eco, HIGH);
  distancia = duracion / 58.2;
  if (distancia < 0 || distancia > 6) {
    distancia = 6;
  }

  //Pequeña = 1; Mediana = 2; Grande = 3
  if (distancia == peq) {
    if (tam == 0)
      tam = 1;
    Serial.println("Pequeña");
    if (pasoColor == true)
      pasoTam = true;
  } else if (distancia == med) {
    if (tam < 2)
      tam = 2;
    Serial.println("Mediana");
    if (pasoColor == true)
      pasoTam = true;
  } else if (distancia == gra ) {
    if (tam < 3)
      tam = 3;
    Serial.println("Grande");
    if (pasoColor == true)
      pasoTam = true;
  } else {
    if (pasoTam == true && pasoColor == true) {
      calcularDireccion = true;
    }
  }
}


void ComprobarColor() {
  int Delay = 100;
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  //Rojo
  Frecuencia = pulseIn(Out, LOW);
  cRojo = map(Frecuencia, 2250, 650, 0, 255);
  delay(Delay);
  //Verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frecuencia = pulseIn(Out, LOW);
  cVerde = map(Frecuencia, 2250, 650, 0, 255);
  delay(Delay);
  //Azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frecuencia = pulseIn(Out, LOW);
  cAzul = map(Frecuencia, 2250, 650, 0, 255);

  /*Serial.print(cRojo);
  Serial.print("  ");
  Serial.print(cVerde);
  Serial.print("  ");
  Serial.print(cAzul);
  Serial.println("   ");*/

  //Rojo = 1; Verde = 2; Azul = 3
  if (cRojo < 0 && cVerde < 0 && cAzul < 60) {
    //Falso Azul
  } else {
    if (cAzul > cRojo && cAzul > cVerde && cRojo < 100 && cAzul >= 60 ) {
      //Azul
      color = 3;
      Serial.println("Azul");
    } else if ((cRojo > 0 && cVerde < 0) || (cRojo > cVerde && cRojo > cAzul) || (cRojo >= 0 && cAzul >= 0 && cVerde < 0)) {
      //Rojo
      color = 1;
      Serial.println("Rojo");
    } else {
      //Verde
      color = 2;
      Serial.println("Verde");
    }
    pasoColor = true;
  }
  delay(Delay);
}

//Cadena Mientras Estan En Funcionamiento
void CadenaFuncionamiento() {
  lcd.setCursor(0, 0);
  lcd.print("G16 - Total");
  lcd.setCursor(0, 1);
  lcd.print("Productos-");
  lcd.setCursor(10, 1);
  lcd.print(ProductosTotales);
  lcd.createChar(0, CaraFeliz);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
}

void CadenaFinalRecipiente1(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.createChar(4,Guion);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16");
  lcd.setCursor(5,0);
  lcd.write((byte)4);
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.print("1");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente1);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

  void CadenaFinalRecipiente2(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.createChar(4,Guion);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16");
  lcd.setCursor(5,0);
  lcd.write((byte)4);
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.print("2");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente1);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

  void CadenaFinalRecipiente3(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.createChar(4,Guion);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16");
  lcd.setCursor(5,0);
  lcd.write((byte)4);
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.print("3");
  lcd.setCursor(2,1);
  lcd.print(ProductosRecipiente1);
  lcd.setCursor(4,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);#include <LiquidCrystal.h>
#include <Stepper.h>
#include "Simbolos.h"
int Frecuencia = 0;

const int stepsPerRevolution = 200;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);
int NumProductos = 100;
//int S1,S0,S2,S3,Out;
int trigger, eco, duracion, distancia;
int peq, med, gra;

int BtnDetener = 44;
//El Estado De La Banda
bool EstaFuncionando;
int tam = 0;
int color = 0;
int ProductosTotales;
int ProductosRecipiente1;
int ProductosRecipiente2;
int ProductosRecipiente3;

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

  ProductosTotales = 0;
  ProductosRecipiente1 = 0;
  ProductosRecipiente2 = 0;
  ProductosRecipiente3 = 0;
    EstaFuncionando=false;
  //Boton Entrada
  pinMode(BtnDetener,INPUT);
  digitalWrite(BtnDetener, HIGH);
  myStepper.setSpeed(35);

  //Control de UltraSonico
  peq = 5;
  med = 4;
  gra = 3;
  trigger = 24;
  eco = 22;

  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);

  //Iniciar Color
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(Out, INPUT);

  //Iniciar Pantalla
  Serial.begin(9600);
  lcd.begin(16, 2);



  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
}

void loop() {
  int Inputs=digitalRead(BtnDetener);
  if(Inputs==0){
    //Se Presiono El Boton
    if(EstaFuncionando){
      lcd.clear();
      CadenaFinalRecipiente1();
      delay(60000);
      lcd.clear();
      CadenaFinalRecipiente2();
      delay(60000);
      lcd.clear();
      CadenaFinalRecipiente3();
      delay(60000);
      EstaFuncionando = false;
    } else {
      EstaFuncionando = true;
    }
  } else {
    lcd.clear();
    CadenaFuncionamiento();
  if (color == 0) {
    ComprobarColor();
  }
  ComprobarDistancia();
  if (calcularDireccion) {
    CalcularDireccion();

    pasoColor = false;
    pasoTam = false;
    tam = 0;
    color = 0;
    calcularDireccion = false;
  }
  }
}

void MoverRecipiente(int numero) {
  if(ProductosTotales == 9){
    ProductosTotales = 0;
  } else {
    ProductosTotales++;
  }
  switch (numero) {
    case 1:
      myStepper.step(-stepsPerRevolution);
      delay(2000);
      myStepper.step(stepsPerRevolution);
      break;
    case 3:
      myStepper.step(stepsPerRevolution);
      delay(2000);
      myStepper.step(-stepsPerRevolution);
      break;
  }
}

void CalcularDireccion() {
  delay(1500);
  switch (color) {
    case 1:
      switch (tam) {
        case 1:
          //Rojo y Pequeña
          //Recipiente 2
          MoverRecipiente(2);
          ProductosRecipiente2++;
          break;
        case 2:
          //Rojo y Mediana
          //Recipiente 1
          MoverRecipiente(1);
          ProductosRecipiente1++;
          break;
        case 3:
          //Rojo y Grande
          //Recipiente 3
          MoverRecipiente(3);
          ProductosRecipiente3++;
          break;
      }
      break;
    case 2:
      switch (tam) {
        case 1:
          //Verde y Pequeña
          //Recipiente 3
          MoverRecipiente(3);
          ProductosRecipiente3++;
          break;
        case 2:
          //Verde y Mediana
          //Recipiente 2
          MoverRecipiente(2);
          ProductosRecipiente2++;
          break;
        case 3:
          //Verde y Grande
          //Recipiente 1
          MoverRecipiente(1);
          ProductosRecipiente1++;
          break;
      }
      break;
    case 3:
      switch (tam) {
        case 1:
          //Azul y Pequeña
          //Recipiente 1
          MoverRecipiente(1);
          ProductosRecipiente1++;
          break;
        case 2:
          //Azul y Mediana
          //Recipiente 3
          MoverRecipiente(3);
          ProductosRecipiente3++;
          break;
        case 3:
          //Azul y Grande
          //Recipiente 2
          MoverRecipiente(2);
          ProductosRecipiente2++;
          break;
      }
      break;
  }
}

void ComprobarDistancia() {
  digitalWrite(trigger, HIGH);
  delay(1);
  digitalWrite(trigger, LOW);

  duracion = pulseIn(eco, HIGH);
  distancia = duracion / 58.2;
  if (distancia < 0 || distancia > 6) {
    distancia = 6;
  }

  //Pequeña = 1; Mediana = 2; Grande = 3
  if (distancia == peq) {
    if (tam == 0)
      tam = 1;
    Serial.println("Pequeña");
    if (pasoColor == true)
      pasoTam = true;
  } else if (distancia == med) {
    if (tam < 2)
      tam = 2;
    Serial.println("Mediana");
    if (pasoColor == true)
      pasoTam = true;
  } else if (distancia == gra ) {
    if (tam < 3)
      tam = 3;
    Serial.println("Grande");
    if (pasoColor == true)
      pasoTam = true;
  } else {
    if (pasoTam == true && pasoColor == true) {
      calcularDireccion = true;
    }
  }
}


void ComprobarColor() {
  int Delay = 100;
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  //Rojo
  Frecuencia = pulseIn(Out, LOW);
  cRojo = map(Frecuencia, 2250, 650, 0, 255);
  delay(Delay);
  //Verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  Frecuencia = pulseIn(Out, LOW);
  cVerde = map(Frecuencia, 2250, 650, 0, 255);
  delay(Delay);
  //Azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  Frecuencia = pulseIn(Out, LOW);
  cAzul = map(Frecuencia, 2250, 650, 0, 255);

  /*Serial.print(cRojo);
  Serial.print("  ");
  Serial.print(cVerde);
  Serial.print("  ");
  Serial.print(cAzul);
  Serial.println("   ");*/

  //Rojo = 1; Verde = 2; Azul = 3
  if (cRojo < 0 && cVerde < 0 && cAzul < 60) {
    //Falso Azul
  } else {
    if (cAzul > cRojo && cAzul > cVerde && cRojo < 100 && cAzul >= 60 ) {
      //Azul
      color = 3;
      Serial.println("Azul");
    } else if ((cRojo > 0 && cVerde < 0) || (cRojo > cVerde && cRojo > cAzul) || (cRojo >= 0 && cAzul >= 0 && cVerde < 0)) {
      //Rojo
      color = 1;
      Serial.println("Rojo");
    } else {
      //Verde
      color = 2;
      Serial.println("Verde");
    }
    pasoColor = true;
  }
  delay(Delay);
}

//Cadena Mientras Estan En Funcionamiento
void CadenaFuncionamiento() {
  lcd.setCursor(0, 0);
  lcd.print("G16 - Total");
  lcd.setCursor(0, 1);
  lcd.print("Productos-");
  lcd.setCursor(10, 1);
  lcd.print(ProductosTotales);
  lcd.createChar(0, CaraFeliz);
  lcd.setCursor(15, 1);
  lcd.write((byte)0);
}

void CadenaFinalRecipiente1(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16-");
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.print("1");
  lcd.setCursor(2,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);
  lcd.print(ProductosRecipiente1);
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

  void CadenaFinalRecipiente2(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16-");
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.setCursor(0,1);
  lcd.print("2");
  lcd.setCursor(2,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);
  lcd.print(ProductosRecipiente2);
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

  void CadenaFinalRecipiente3(){
  lcd.createChar(0,Dolar);
  lcd.createChar(1,Interrogacion);
  lcd.createChar(2,DoblePunto);
  lcd.createChar(3,Amperson);
  lcd.setCursor(0,0);
  lcd.write((byte)1);
  lcd.setCursor(1,0);
  lcd.write((byte)2);
  lcd.setCursor(2,0);
  lcd.print("G16-");
  lcd.setCursor(6,0);
  lcd.print("Recipiente");
  lcd.setCursor(0,1);
  lcd.setCursor(0,1);
  lcd.print("3");
  lcd.setCursor(2,1);
  lcd.print("Productos-");
  lcd.setCursor(13,1);
  lcd.print(ProductosRecipiente3);
  lcd.setCursor(14,1);
  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

  lcd.write((byte)0);
  lcd.setCursor(15,1);
  lcd.write((byte)3);
  }

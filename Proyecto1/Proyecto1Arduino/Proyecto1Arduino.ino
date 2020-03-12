#include <LedControl.h>
#include <LiquidCrystal.h>

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
const int BTNS = 10;


int PENDIENTE = 0;
//
int sensorReading=0;//Pin análogo en espera
//Pines Analogos De Lectura
int MonedaQuetzal=0;
int MonedaVeinti=1;
int MonedaCincuenta=2;
int DelayMonedas=300;
//Variables Del Contador
int Credito=0;//Credito Actual
int UltimaMoneda=0;//25,50,100
int CantidadMonedas100=0;
int CantidadMonedas50=0;
int CantidadMonedas25=0;
//Pantalla LCD
int RS=22;
int Enable=24;
int D4=26;
int D5=28;
int D6=30;
int D7=32;
//Modalidad Bluetooth
//Variables
int ModoDeJuego=0;
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7
);
//
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
  pinMode(BTNS, INPUT);
  //Bluetooth Y Contador
  Serial2.begin(9600);
 Serial.begin(9600);
 lcd.begin(16, 2);
  // Print a message to the LCD.
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Credito Actual ");
}

//True -> subir
//False -> bajar
void moverZ(bool tipo){
  if(tipo){
    digitalWrite(motor3,HIGH);
    digitalWrite(motor3a,LOW);
    delay(PENDIENTE);
    digitalWrite(motor3,LOW);
    digitalWrite(motor3a,LOW);
  }else{
    digitalWrite(motor3,LOW);
    digitalWrite(motor3a,HIGH);
    delay(PENDIENTE);
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
      digitalWrite(motor1,LOW);
      digitalWrite(motor1a,LOW);
      posicionY++;
    }
  }else{
    if(posicionY != 0){
      digitalWrite(motor2,LOW);
      digitalWrite(motor2a,HIGH);
      delay(375);
      digitalWrite(motor1,LOW);
      digitalWrite(motor1a,LOW);
      posicionY--;
    }
  }
  lc.setLed(0, posicionY, posicionX, true);
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
  AccionesDelContador();
  //Bluetooth
  if (Serial2.available())
  {  
      OperacionesBluetooth();
       
  }

  
if(ModoDeJuego==1){
  if(digitalRead(BTNU) == HIGH){
    moverY(true);
  }
  if(digitalRead(BTND == HIGH)){
    moverY(false);
  }
  if(digitalRead(BTNR) == HIGH){
    moverX(true);
  }
  if(digitalRead(BTNL == HIGH)){
    moverX(false);
  }
  if(digitalRead(BTNS) == HIGH){
    moverZ(true);
  }
  if(digitalRead(BTNB == HIGH)){
    moverZ(false);
  }
  }
}



//Para El Contador
void AccionesDelContador(){
  sensorReading=analogRead(0); //Instrucción para obtener dato analógico
 if(sensorReading<400){
    Serial.print("Uno   "); 
    Serial.println(sensorReading); 
    Credito=Credito+100;
    delay(DelayMonedas);
  }

  sensorReading=analogRead(1); //Instrucción para obtener dato analógico
 if(sensorReading<385){
    Serial.print("Cincuenta   "); 
    Serial.println(sensorReading);
    Credito=Credito+50;
    delay(DelayMonedas);
  }



  
   
  sensorReading=analogRead(2); //Instrucción para obtener dato analógico
 if(sensorReading<385){
    Serial.print("Veinticinco   "); 
    Serial.println(sensorReading);
    Credito=Credito+25;
    delay(DelayMonedas);
  }

  lcd.setCursor(0, 1);
  lcd.print(Credito);
 /*
  Serial.print("1:");
  Serial.print(analogRead(MonedaQuetzal));
  Serial.print("    ");
  Serial.print("50:");
  
  Serial.print(analogRead(MonedaVeinti));
  Serial.print("    ");
  
  Serial.print("25:");
  Serial.print(analogRead(MonedaCincuenta));
  Serial.println("");
  */
}
//Operaciones Bluetooth
bool ModoRemoto(char m){
  if(ModoDeJuego!=2)//No Esta En Modo Remoto
      return true;
  
        if(m=='u'){//MoverArriba
          moverY(true);
         return true;
        }else
        if(m=='d'){//MoverAbajo
          moverY(false);
         return true;
        }else
        if(m=='l'){//MoverIzquierda
         moverX(false);
         return true;
        }else
        if(m=='r'){//MoverDerecha
            //MoverDerecha();
          moverX(true);
          return true;
        }else
        if(m=='g'){//Agarrar
         return true;
        }else
        if(m=='s'){//Soltar
          return true;
        }
        return false;
}
void OperacionesBluetooth(){
  bool Continuar=true;
        //Serial2.print(450);
        char m=Serial2.read();
        Serial.println(m);
        if(m=='y'){//Desactivar Modos
          ModoDeJuego=0;
        }else
        if(m=='x'){//ModoManual
          ModoDeJuego=1;
        }else
        if(m=='z'){//ModoRemoto
          ModoDeJuego=2;
        }
        else if(m=='i'){//Enviar Info Monedas
            if(Credito<0)
              Credito=0;
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
        }else if(m=='c'){//Comprar Boleto
            Credito=Credito-100;
        }else{
        //delay(1000);//Delay Para Que No Duplique Informacion
        if(!ModoRemoto){
          Serial.println("Desconocido");//Bluetooth Desconocido
          Serial.println(m);
        }
        /*char hola[2];
        hola[0]='n';
        hola[1]='o';
        */
        }
  
}

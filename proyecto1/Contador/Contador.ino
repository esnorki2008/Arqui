#include <LiquidCrystal.h>
//ContadorDeMonedas

int sensorReading=0;//Pin análogo en espera
//Pines Analogos De Lectura
int MonedaQuetzal=10;
int MonedaVeinti=9;
int MonedaCincuenta=11;
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
//

LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7
);



void setup()
{
 Serial2.begin(9600);
 Serial.begin(9600);
 lcd.begin(16, 2);
  // Print a message to the LCD.
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Credito Actual ");
}
//Para El Contador
void AccionesDelContador(){
  sensorReading=analogRead(MonedaQuetzal); //Instrucción para obtener dato analógico
 if(sensorReading<280){
    Serial.print("Uno   "); 
    Serial.println(sensorReading); 
    Credito=Credito+100;
    delay(DelayMonedas);
  }

  sensorReading=analogRead(MonedaCincuenta); //Instrucción para obtener dato analógico
 if(sensorReading<280){
    Serial.print("Cincuenta   "); 
    Serial.println(sensorReading);
    Credito=Credito+50;
    delay(DelayMonedas);
  }



  
   
  sensorReading=analogRead(MonedaVeinti); //Instrucción para obtener dato analógico
 Serial.println(sensorReading);
   if(sensorReading<495){
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
         return true;
        }else
        if(m=='d'){//MoverAbajo
         return true;
        }else
        if(m=='l'){//MoverIzquierda
         return true;
        }else
        if(m=='r'){//MoverDerecha
            //MoverDerecha();
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
void loop()
{
  AccionesDelContador();
  if (Serial2.available())
  {  
      OperacionesBluetooth();
       
  }
 
  
}

//ContadorDeMonedas
int sensorReading=0;//Pin análogo en espera
int MonedaQuetzal=0;
int MonedaVeinti=1;
int MonedaCincuenta=2;
int Credito=0;
//Pantalla LCD
int RS=22;
int Enable=24;
int D4=26;
int D5=28;
int D6=30;
int D7=32;
//
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7
);



void setup()
{
 Serial.begin(9600);
 lcd.begin(16, 2);
  // Print a message to the LCD.
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Credito Actual ");
}
void loop()
{


 sensorReading=analogRead(0); //Instrucción para obtener dato analógico
 if(sensorReading<400){
    Serial.print("Uno   "); 
    Serial.println(sensorReading); 
    Credito=Credito+100;
    delay(100);
  }

  sensorReading=analogRead(1); //Instrucción para obtener dato analógico
 if(sensorReading<385){
    Serial.print("Cincuenta   "); 
    Serial.println(sensorReading);
    Credito=Credito+50;
    delay(100);
  }



  
   
  sensorReading=analogRead(2); //Instrucción para obtener dato analógico
 if(sensorReading<385){
    Serial.print("Veinticinco   "); 
    Serial.println(sensorReading);
    Credito=Credito+25;
    delay(100);
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

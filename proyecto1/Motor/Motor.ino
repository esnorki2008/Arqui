#include <Stepper.h>
#include "Simbolos.h"
int Frecuencia = 0;
int veces = 0;

const int stepsPerRevolution = 300;
Stepper myStepper = Stepper(stepsPerRevolution, 52, 48, 50, 46);

void setup() {

 
  myStepper.setSpeed(100);

}

void loop() {

 if(veces == 0){
  //Abrir
 for(int i=0;i<20;i++)
 {
    //delay(100);
    myStepper.step(-stepsPerRevolution);
 }
 delay(3000);
 //Cerrar 
 for(int i=0;i<20;i++)
 {
    //delay(100);
    myStepper.step(stepsPerRevolution);
 }
 veces++;
 }
}

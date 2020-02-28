#include <EEPROM.h>
 
String c = "";

int DirecEEprom = 0;
struct Directiva{
  int  Direccion;
  int Tiempo;
}typedef Dir;


struct Rutas{
  char Nombre[5];
  Directiva Pasos[10];
}typedef Rut;

struct ConjuntoRutas{
  Rut Rutas[3];
}typedef CR;

CR EEprom;
void EscribirEEprom(){
  EEPROM.put( DirecEEprom, EEprom);
}

void EnviarRutaGuardada(){
  EEPROM.get( DirecEEprom, EEprom ); 
  for(int i=0;i<3;i++){
 
      Rut NuevaRuta=EEprom.Rutas[i];
      
      for(int x=0;x<5;x++){
        Serial2.print(NuevaRuta.Nombre[x]);
        Serial.print(NuevaRuta.Nombre[x]);
      }
      
      for(int x=0;x<10;x++){
        Directiva Paso=NuevaRuta.Pasos[x];
        
        Serial2.print(",");
        Serial2.print(Paso.Direccion);
        Serial2.print(",");
        Serial2.print(Paso.Tiempo);

       Serial.print(",");
       Serial.print(Paso.Direccion);
       Serial.print(",");
       Serial.print(Paso.Tiempo);
      }
      Serial2.print("}");

      Serial.println("}");
   }
    Serial2.print("$");

    Serial.println("$");
}
void setup() 
{
    Serial2.begin(9600);
    Serial.begin(9600);

   
   
   for(int i=0;i<3;i++){
      Rut NuevaRuta;
      NuevaRuta.Nombre[0]='R';
      NuevaRuta.Nombre[1]='U';
      NuevaRuta.Nombre[2]='T';
      NuevaRuta.Nombre[3]='A';
      NuevaRuta.Nombre[4]='Z';
  
      for(int x=0;x<10;x++){
        Directiva Paso;
        Paso.Direccion=i;
        Paso.Tiempo=i+10;
        NuevaRuta.Pasos[x]=Paso;
      }
      EEprom.Rutas[i]=NuevaRuta;
   }
  
  
  
 EscribirEEprom();



   
  
}
 
void loop()
{
    
    if (Serial2.available())
    {  

      EnviarRutaGuardada();
     
        //Serial2.print(450);
        String m=Serial2.readString();
        delay(1000);//Delay Para Que No Duplique Informacion
        
        Serial.println(m);
        /*char hola[2];
        hola[0]='n';
        hola[1]='o';
        */
     
        
    }
 
}

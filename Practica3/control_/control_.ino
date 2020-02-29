#include <EEPROM.h>
//Structs Para EEprom
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
//Manejo EEprom
CR EEprom;
String Entrada = "";
int RutaActual[20];
int ContaEEprom=0;//Contador De Rutas Aagregadas
int MarcaDirecciones=0;//Contador De Direcciones
int MarcaTiempo=0;//Contador De Direcciones
int DirecEEprom = 0;



void EscribirEEprom(){
  Serial.println("Escrbiendo EEPROM");
  EEPROM.put( DirecEEprom, EEprom);
}
void GenerarRutaNula(int Indice){
  Rut RutaNula;
  RutaNula.Nombre[0]='n';
  RutaNula.Nombre[1]='u';
  RutaNula.Nombre[2]='l';
  RutaNula.Nombre[3]='l';
  RutaNula.Nombre[4]='p';
      for(int x=0;x<10;x++){
        Directiva Paso;
        Paso.Direccion=0;
        Paso.Tiempo=0;
        RutaNula.Pasos[x]=Paso;
      }
  EEprom.Rutas[Indice]=RutaNula;
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
      if(i==0)
      NuevaRuta.Nombre[4]='0';
      if(i==1)
      NuevaRuta.Nombre[4]='1';
      if(i==2)
      NuevaRuta.Nombre[4]='2';
  
      for(int x=0;x<10;x++){
        Directiva Paso;
        Paso.Direccion=i;
        Paso.Tiempo=i+10;
        NuevaRuta.Pasos[x]=Paso;
      }
      EEprom.Rutas[i]=NuevaRuta;
   }
  
  
  
//EscribirEEprom();



   
  
}

void ComprobarEEprom(){//Para Ver Si Los Datos Se Ingresaron Bien
  for(int i=0;i<3;i++){ 
      Rut NuevaRuta=EEprom.Rutas[i];     
      for(int x=0;x<5;x++){       
        Serial.print(NuevaRuta.Nombre[x]);
      }    
      for(int x=0;x<10;x++){
        Directiva Paso=NuevaRuta.Pasos[x];        
       Serial.print(",");
       Serial.print(Paso.Direccion);
       Serial.print(",");
       Serial.print(Paso.Tiempo);
      }
      Serial.println("}");
   }
    Serial.println("$");
}
void OperacionesBluetooth(){
  bool Continuar=true;
        //Serial2.print(450);
        char m=Serial2.read();
        if(m=='z'){//CrearNuevaRuta
          int Contador=0;
          
          while(Continuar){   
            //Serial.println("loop");          
            if(Serial2.available()){
              char C=Serial2.read();
              if(C==','){//Delimitador Entre Bytes
                if(Contador==0){//ElNombre
                  EEprom.Rutas[ContaEEprom].Nombre[0]=Entrada[0];
                  EEprom.Rutas[ContaEEprom].Nombre[1]=Entrada[1];
                  EEprom.Rutas[ContaEEprom].Nombre[2]=Entrada[2];
                  EEprom.Rutas[ContaEEprom].Nombre[3]=Entrada[3];
                  EEprom.Rutas[ContaEEprom].Nombre[4]=Entrada[4];
                }else{//Contenido Ruta
                  MarcaDirecciones++;
                  MarcaTiempo++;
                  if(Contador%2==0){//Es Una Direccion
                    EEprom.Rutas[ContaEEprom].Pasos[MarcaDirecciones].Direccion=Entrada.toInt();                    
                  }else{//Es Un Tiempo
                    EEprom.Rutas[ContaEEprom].Pasos[MarcaTiempo].Tiempo=Entrada.toInt();
                    
                  }                 
                  Entrada="";
                 
                  if(MarcaDirecciones>10)
                      MarcaDirecciones=0;
                  if(MarcaTiempo>10)
                      MarcaTiempo=0;
                }                                                     
                Contador++;
              }else{//Acumulador
                  Entrada=Entrada+C;                   
              }
            }
            if(Contador>20){//21 Bytes De La Ruta
                ContaEEprom++;
                if(ContaEEprom>2){
                  ContaEEprom=0;
                  ComprobarEEprom();
                }
                Continuar=false;
            }
          }
          Serial.println();
        }else
        if(m=='n'){//Arriba
        }else
        if(m=='s'){//Abajo
        }else
        if(m=='e'){//Izquieda
        }else
        if(m=='o'){//derecha
        }else
        if(m=='t'){//limpiar
        }else
        if(m=='b'){//BorrarStruct
          while(Continuar){
            if(Serial2.available()){
              char C=Serial2.read();
              Entrada=C;
              GenerarRutaNula(Entrada.toInt());
              EscribirEEprom();           
              Continuar=false;
              Entrada="";
              
              }       
          } 
          
          
        }
        else if(m=='i'){//Enviar Structs
          EnviarRutaGuardada();
        }else if(m=='r'){//Reconocer Movimientos
          int Contador=0;
          
          while(Continuar){   
            //Serial.println("loop");          
            if(Serial2.available()){
              char C=Serial2.read();
              if(C==','){//Delimitador Entre Bytes
                RutaActual[Contador]=Entrada.toInt(); 
                Entrada="";
                                         
                Contador++;
              }else{//Acumulador
                  Entrada=Entrada+C;                   
              }
            }
            if(Contador>19)//20 Bytes De La Ruta
              Continuar=false;
          }
 
          for(int i=0;i<=18;i=i+2){
            //FALTA MOVER EN BASE A VALORES
            Serial.print(RutaActual[i]);
            Serial.print("  ");
            Serial.println(RutaActual[i+1]);
          }
          
        }else{
        //delay(1000);//Delay Para Que No Duplique Informacion
        Serial.println("Descibicudi");//Bluetooth Desconocido
        Serial.println(m);
        /*char hola[2];
        hola[0]='n';
        hola[1]='o';
        */
        }
  
}
void loop()
{
  
    
    if (Serial2.available())
    {  
      OperacionesBluetooth();
        
    }
 
}

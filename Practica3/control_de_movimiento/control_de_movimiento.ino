#include <EEPROM.h>
//Motores
const int controlpin1a =8;
const int controlpin2a=9;
const int controlpin1b =6;
const int controlpin2b=7;

//Ultrasonicos
const int echoAdelante = 24;
const int trigAdelante = 25;
const int echoIzquierda = 28;
const int trigIzquierda = 29;
const int echoDerecha = 26;
const int trigDerecha = 27;
const int minimo = 20;
int duracion;
int distancia;

//Color
const int S0 = 41;
const int S1 = 39;
const int S2 = 45;
const int S3 = 47;
const int Out = 49;
int cRed, cGreen, cBlue;

//Barrido
const int barredora = 53;

//Boton
const int boton = 8;
boolean presionado;

//EEprom
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
int MarcaPasos=0;//Contador De Direcciones
int DirecEEprom = 0;





void setup(){
  //Inicializaciones
  duracion = 0;
  distancia = 0;
  cRed = 0;
  cGreen = 0;
  cBlue = 0;
  
  //Motores
  pinMode(controlpin1a,OUTPUT); 
  pinMode(controlpin2a,OUTPUT);
  pinMode(controlpin1b,OUTPUT);
  pinMode(controlpin2b,OUTPUT);

  //Triggers
  pinMode(trigAdelante, OUTPUT);
  pinMode(trigIzquierda, OUTPUT);
  pinMode(trigDerecha, OUTPUT);

  //Echos
  pinMode(echoAdelante, INPUT);
  pinMode(echoIzquierda, INPUT);
  pinMode(echoDerecha, INPUT);

  //Color
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(Out, INPUT);
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  //Barredora
  pinMode(barredora, OUTPUT);
  digitalWrite(barredora, HIGH);

  //Boton
  pinMode(boton, INPUT);
  presionado = false;

  Serial.begin(9600);
  //Bluetooth
  Serial2.begin(9600);  
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
  
  
  
EscribirEEprom();
}

boolean verificarAdelante(){
  digitalWrite(trigAdelante, HIGH);
  delay(1);
  digitalWrite(trigAdelante, LOW);

  duracion = pulseIn(echoAdelante, HIGH);
  distancia = duracion / 58.2;
  /*Serial.print("Distancia adelante ");
  Serial.print(distancia);
  Serial.println(" ");*/
  if(distancia <= minimo){
    return false;
  }
  return true;
}

boolean verificarIzquierda(){
  digitalWrite(trigIzquierda, HIGH);
  delay(1);
  digitalWrite(trigIzquierda, LOW);
  
  duracion = pulseIn(echoIzquierda, HIGH);
  distancia = duracion / 58.2;
  /*Serial.print("Distancia izquierda ");
  Serial.print(distancia);
  Serial.println(" ");*/
  if(distancia <= minimo){
    return false;
  }
  return true;
}

boolean verificarDerecha(){
  digitalWrite(trigDerecha, HIGH);
  delay(1);
  digitalWrite(trigDerecha, LOW);

  duracion = pulseIn(echoDerecha, HIGH);
  distancia = duracion / 58.2;
  /*Serial.print("Distancia derecha ");
  Serial.print(distancia);
  Serial.println(" ");*/
  if(distancia <= minimo){
    return false;
  }
  return true;
}

void derecha(int derecha){
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,HIGH); 
  digitalWrite(controlpin1b,HIGH);
  digitalWrite(controlpin2b,LOW); 
  delay(derecha);
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,LOW);  
}
  
void izquierda(int izquierda){
  digitalWrite(controlpin1a,HIGH);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,HIGH); 
  delay(izquierda);
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,LOW);  
}
  
void abajo(int abajo){
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,HIGH); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,HIGH); 
  delay(abajo);
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,LOW); 
}

void arriba(int arriba){
  digitalWrite(controlpin1a,HIGH);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,HIGH);
  digitalWrite(controlpin2b,LOW);
  delay(arriba);
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,LOW);  
}

void comprobarDistancias(){
  if(verificarAdelante()){
    arriba(1000);
    Serial.println("Moviendo el carro hacia adelante");
  } else {
    if(verificarIzquierda()){
      izquierda(2000);
      Serial.println("Moviendo el carro hacia izquierda");
    } else {
      if(verificarDerecha()){
        derecha(2000);
        Serial.println("Moviendo el carro hacia derecha");
      } else {
        abajo(1000);
        Serial.println("Moviendo el carro hacia atras");
      }
    }
  }
}

int comprobarColor(){
  //Rojo
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  cRed = map(pulseIn(Out, LOW), 2250, 650, 0, 255);
  delay(100);

  //Verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  cGreen = map(pulseIn(Out, LOW), 2250, 650, 0, 255);
  delay(100);

  //Azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  cBlue = map(pulseIn(Out, LOW), 2250, 650, 0, 255);
  delay(100);

  /*Serial.print("Rojo ");
  Serial.print(cRed);
  Serial.print("    Verde ");
  Serial.print(cGreen);
  Serial.print("    Azul ");
  Serial.print(cBlue);
  Serial.println("");*/
  

  if(cRed < 100 && cGreen < 100){
    Serial.println("GRADA");
    return 0;
  } else if(cBlue > 300){
    Serial.println("MANUAL");
    return 1;
  } else if(cRed > cBlue && cRed > cGreen){
    Serial.println("BARRER");
    return 2;
  } else {
    Serial.println("AUTOMATICO");
    return 3;
  }
}

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

void ComprobarEEprom(){//Para Ver Si Los Datos Se Ingresaron Bien
    
  for(int i=0;i<3;i++){ 
    Serial.println();
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
    
   }
  
}
void MovimientoAutomatico(int Direccion,int Tiempo){
  switch(Direccion){
    case 0:
      arriba(Tiempo);
      break;
    case 1:
      abajo(Tiempo);
      break;
    case 2:
      izquierda(Tiempo);
      break;
    case 3:
      derecha(Tiempo);
      break;  
  }
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
                  Serial.print(Entrada);
                  
                }else{//Contenido Ruta
                  Serial.print(",");
                  //Serial.print(Entrada);
                  
                  if(Contador%2!=0){//Es Una Direccion   
                    EEprom.Rutas[ContaEEprom].Pasos[MarcaPasos].Direccion=Entrada.toInt();
                    //Serial.print(MarcaPasos);                                       
                  }else{//Es Un Tiempo     
                    EEprom.Rutas[ContaEEprom].Pasos[MarcaPasos].Tiempo=Entrada.toInt();
                    //Serial.print(MarcaPasos); 
                    MarcaPasos++;
                  }                                       
                  if(MarcaPasos>9)
                      MarcaPasos=0;
                 
                }
                Entrada="";                                                     
                Contador++;
              }else{//Acumulador
                  Entrada=Entrada+C;                   
              }
            }
            if(Contador>20){//21 Bytes De La Ruta
                ContaEEprom++;
                if(ContaEEprom>2){
                  MarcaPasos=0;
                  ContaEEprom=0;
                  EscribirEEprom();//Escribir Los Datos Enviados
                  //ComprobarEEprom();
                }
                Continuar=false;
            }
          }
          Serial.println();
        }else
        if(m=='n'){//Arriba
          arriba(1000);
        }else
        if(m=='s'){//Abajo
          abajo(1000);
        }else
        if(m=='e'){//Izquieda
          izquierda(1000);
        }else
        if(m=='o'){//derecha
          derecha(1000);
        }else
        if(m=='t'){//limpiar
          digitalWrite(barredora, LOW);
          delay(10000);
          digitalWrite(barredora, HIGH);
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
            MovimientoAutomatico(RutaActual[i],RutaActual[i+1]*1000);
            //
            Serial.print(RutaActual[i]);
            //Serial.print("  ");
            //Serial.println(RutaActual[i+1]);
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
  
void loop(){
  if(digitalRead(boton) == 1){
    presionado = !presionado;
  }
  if(!presionado){
    switch(comprobarColor()){
      //Color negro, Retroceso
      case 0:
        abajo(100);
        digitalWrite(barredora, HIGH);
      break;
      //Color azul, Manual
      case 1:
        //Codigo de control manual
        if (Serial2.available())
        {  
        OperacionesBluetooth();        
        }
        
        digitalWrite(barredora, HIGH);
      break;
      //Color rojo, barrer
      case 2:
        //Codigo de barrido
        digitalWrite(barredora, LOW);
        comprobarDistancias();
      break;
      //Sin color
      case 3:
        digitalWrite(barredora, HIGH);
        comprobarDistancias();
      break;
    }
  } else {
    //Modo Manual
  }
  delay(50);
}

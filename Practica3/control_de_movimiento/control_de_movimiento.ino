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

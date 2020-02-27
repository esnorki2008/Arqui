//Motores
const int controlpin1a =4;
const int controlpin2a=5;
const int controlpin1b =6;
const int controlpin2b=7;

//Ultrasonicos
const int echoAdelante = 24;
const int trigAdelante = 25;
const int echoIzquierda = 26;
const int trigIzquierda = 27;
const int echoDerecha = 28;
const int trigDerecha = 29;
const int minimo = 3;
int duracion;
int distancia;

//Color
const int S0 = 10;
const int S1 = 11;
const int S2 = 12;
const int S3 = 13;
const int Out = 14;
int cRed, cGreen, cBlue;

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

  Serial.begin(9600);
}

boolean verificarAdelante(){
  digitalWrite(trigAdelante, HIGH);
  delay(1);
  digitalWrite(trigAdelante, LOW);

  duracion = pulseIn(echoAdelante, HIGH);
  distancia = duracion / 58.2;
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
  if(distancia <= minimo){
    return false;
  }
  return true;
}

void izquierda(int izquierda){
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,HIGH); 
  digitalWrite(controlpin1b,HIGH);
  digitalWrite(controlpin2b,LOW); 
  delay(izquierda);
  digitalWrite(controlpin1a,LOW);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,LOW);  
}
  
void derecha(int derecha){
  digitalWrite(controlpin1a,HIGH);
  digitalWrite(controlpin2a,LOW); 
  digitalWrite(controlpin1b,LOW);
  digitalWrite(controlpin2b,HIGH); 
  delay(derecha);
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
    arriba(10);
    Serial.println("Moviendo el carro hacia adelante");
  } else {
    if(verificarIzquierda()){
      izquierda(10);
      Serial.println("Moviendo el carro hacia izquierda");
    } else {
      if(verificarDerecha()){
        derecha(10);
        Serial.println("Moviendo el carro hacia derecha");
      } else {
        abajo(10);
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

  /*if(COMBINACIONES NEGRO){
    return 0;
  } else if(COMBINACIONES AZUL){
    return 1;
  } else if(COMBINACIONES ROJO){
    return 2;
  } else {
    return 3;
  }*/
}
  
void loop(){
  switch(comprobarColor()){
    //Color negro, Retroceso
    case 0:
      abajo(100);
    break;
    //Color azul, Manual
    case 1:
      //Codigo de control manual
    break;
    //Color rojo, barrer
    case 2:
      //Codigo de barrido
      comprobarDistancias();
      break;
    //Sin color
    case 3:
      comprobarDistancias();
    break;
  }
  delay(50);
}

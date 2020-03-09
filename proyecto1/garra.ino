//Motores
const int motor1 = 47;
const int motor1a = 49;

const int motor2 = 51;
const int motor2a = 53;

const int motor3 = 44;
const int motor3a = 45;

void setup() {
   pinMode(motor1,OUTPUT);
     pinMode(motor1a,OUTPUT); 
       pinMode(motor2,OUTPUT); 
         pinMode(motor2a,OUTPUT); 
           pinMode(motor3,OUTPUT); 
             pinMode(motor3a,OUTPUT); 
              digitalWrite(motor3,LOW);
             digitalWrite(motor3a,LOW);
             
                

}
void subir(int subir){
  digitalWrite(motor3,HIGH);
  digitalWrite(motor3a,LOW);
  subir = subir* 1.24;
  delay(subir);
  digitalWrite(motor3,LOW);
  digitalWrite(motor3a,LOW);
  }

  void bajar(int bajar){
  digitalWrite(motor3a,HIGH);
  digitalWrite(motor3,LOW);
  delay(bajar);
  digitalWrite(motor3,LOW);
  digitalWrite(motor3a,LOW);
  }
  void xmas(int subir){
  digitalWrite(motor1,HIGH);
  digitalWrite(motor1a,LOW);
  subir=subir*585;
  delay(subir);
  digitalWrite(motor1,LOW);
  digitalWrite(motor1a,LOW);
  }

  void xmenos(int bajar){
  digitalWrite(motor1a,HIGH);
  digitalWrite(motor1,LOW);
bajar= bajar*600;
  delay(bajar);
  digitalWrite(motor1,LOW);
  digitalWrite(motor1a,LOW);
  }
    void ymas(int subir){
  digitalWrite(motor2,HIGH);
  digitalWrite(motor2a,LOW);
  subir = subir *375;
  delay(subir);
  digitalWrite(motor2,LOW);
  digitalWrite(motor2a,LOW);
  }

  void ymenos(int bajar){
  digitalWrite(motor2a,HIGH);
  digitalWrite(motor2,LOW);
  bajar = bajar*375;
  delay(bajar);
  digitalWrite(motor2,LOW);
  digitalWrite(motor2a,LOW);
  }
void loop() {
  
 

     xmas(7);
  delay(2000); 
    xmenos(7  );
  delay(2000); 

}

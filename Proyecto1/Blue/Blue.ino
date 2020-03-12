int Creditos=1000;
int UltimaMoneda=0;
int CantidadMonedas100=0;
int CantidadMonedas50=0;
int CantidadMonedas25=0;
int ModoDeJuego=0;
void setup() 
{
    Serial2.begin(9600);
    Serial.begin(9600);  
}
void loop()
{
  
    
    if (Serial2.available())
    {  
      OperacionesBluetooth();
       
    }
 
}
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
            if(Creditos<0)
              Creditos=0;
            Serial2.print(Creditos);
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
            Creditos=Creditos-100;
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

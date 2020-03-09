
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
void OperacionesBluetooth(){
  bool Continuar=true;
        //Serial2.print(450);
        char m=Serial2.read();
        if(m=='u'){//MoverArriba
         
        }
        if(m=='d'){//MoverAbajo
         
        }
        if(m=='l'){//MoverIzquierda
         
        }
        if(m=='r'){//MoverDerecha
            //MoverDerecha();
        }
        if(m=='g'){//Agarrar
         
        }
        if(m=='s'){//Soltar
         
        }
        if(m=='t'){//EstablecerModoDeJuego
         
        }
        else if(m=='i'){//Enviar Monedas
            Serial2.print(700);
            Serial2.print('$');
            Serial.println("Hola");
        }else if(m=='c'){//Comprar Boleto
            Serial2.write(100);
        }else{
        //delay(1000);//Delay Para Que No Duplique Informacion
        Serial.println("Desconocido");//Bluetooth Desconocido
        Serial.println(m);
        /*char hola[2];
        hola[0]='n';
        hola[1]='o';
        */
        }
  
}

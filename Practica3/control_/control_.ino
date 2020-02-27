
 
String c = "";
 
void setup() 
{
    Serial2.begin(9600);
    Serial.begin(9600);
  
}
 
void loop()
{
    String m=Serial2.readString();
    if (m!=-1)
    {  
     
        //Serial2.write(c);
   
        delay(1000);//Delay Para Que No Duplique Informacion
        
        Serial.println(m);
        //char hola[2];
        //Serial2.write(hola);
     
        
    }
 
}

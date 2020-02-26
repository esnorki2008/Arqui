
 
int c = 0;
 
void setup() 
{
    Serial2.begin(9600);
    Serial.begin(9600);
  
}
 
void loop()
{
    int m=Serial2.read();
    if (m!=-1)
    {  
     
        //Serial2.write(c);
   
        delay(1000);//Delay Para Que No Duplique Informacion
        
        Serial.println(m);
        Serial2.write(m);
     
        
    }
 
}

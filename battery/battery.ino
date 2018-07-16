#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins

#include <OneWire.h>                 //Se importan las librerías
#include <DallasTemperature.h>
 
//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial BT(10,11); //10 RX, 11 TX.

#define Pin 2  
OneWire ourWire(Pin);                //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se llama a la librería DallasTemperature
char cadena[555];
int i=0;

#define NUM_SAMPLES 10 //num samples per reading
int sum = 0; //sum of samples taken
unsigned char sample_count = 0; //current sample number
float voltage = 0.0 ;


 
void setup()
{
  delay(1000);
  BT.begin(9600);
  Serial.begin(9600);
  //sensors.begin();
  
}
 
void loop()
{
  
  if(BT.available())
  {
    char dato=BT.read();
    cadena[i++]=dato;
    if(dato=='\n')
    {
     
      Serial.print(cadena);
      if(strstr(cadena,"start")!=0)
      {
        while(true)
        {
          if(strstr(cadena,"stop")==0)
        {
              sensors.begin();
              sensors.requestTemperatures();       //Prepara el sensor para la lectura

              BT.println("Data:");
              BT.println(sensors.getTempCByIndex(0));
              //BT.println(" Graus centigrads");
               
              delay(1);  
              
              //int sensorValue = analogRead(A0);
              //float voltage = sensorValue * (5.0 / 1023.0)*10;
              
              while (sample_count < NUM_SAMPLES) {
                sum += analogRead(A0);
                sample_count++;
                delay(10);
              }
           
          
              voltage = ((float)sum / (float)NUM_SAMPLES * 5) / 1023.0;
              BT.print(voltage * 11);
              BT.println(" V");
              sample_count = 0;
              sum = 0;
              BT.println();
              

              delay(3000);

   
    }
    char dato=BT.read();
    cadena[i++]=dato;
      }
      }
      }
}
}


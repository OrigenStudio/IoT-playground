

#include <SPI.h>
#include <WiFi101.h>

#include <OneWire.h>                 //Se importan las librerías
#include <DallasTemperature.h>

const String APIKEY = "1b380e504a3f7a45ec646b5bd7f5879c96ff9abadbae44d60ad5ebb3d91eb193"; ///your apikey
const String DEVICE = "mkr1000@paulamb.paulamb"; // your deviceID

 
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
//char password[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
const char* server = "api.carriots.com";
WiFiClient client;
int status = WL_IDLE_STATUS;

 
#define Pin 5                        //Se declara el pin donde se conectará la DATA
OneWire ourWire(Pin);                //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se llama a la librería DallasTemperature
 
void setup() {
 sensors.begin();
 Serial.begin(115200);
 delay(1000);
 // start wifi
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}
// Send data to Carriot
 
void sendStream()
{
 //const int httpPort = 80;
 if (client.connect(server, 80)) { // If there's a successful connection
 Serial.println(F("connected"));
 
 sensors.requestTemperatures();
 Serial.println(sensors.getTempCByIndex(0)); 
 String temperature = String(sensors.getTempCByIndex(0));
 
 // construct the data json
 String json = "{\"protocol\":\"v2\",\"device\":\"" + DEVICE + "\",\"at\":\"now\",\"data\":{\"temperature\":\"" + temperature + "\"}}";
 
 // Make an HTTP request
 client.println("POST /streams HTTP/1.1");
 client.println("Host: api.carriots.com");
 client.println("Accept: application/json");
 client.println("User-Agent: Arduino-Carriots");
 client.println("Content-Type: application/json");
 client.print("carriots.apikey: ");
 client.println(APIKEY);
 client.print("Content-Length: ");
 int thisLength = json.length();
 client.println(thisLength);
 client.println("Connection: close");
 client.println();
 client.println(json);
 
 
 
 while(client.available()){
 String line = client.readStringUntil('\r');
 Serial.print(line);
 }
 
 
 }
 else {
 // If server connection failed:
 Serial.println(F("connection failed"));
 }
}
void loop() {
 
 Serial.println(F("Enviando...."));
 sendStream();
 delay(5000);
}

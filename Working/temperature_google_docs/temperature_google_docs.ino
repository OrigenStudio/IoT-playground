//https://www.hackster.io/detox/send-mkr1000-data-to-google-sheets-1175ca

#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include <WiFiClient.h>
#include <SPI.h>

#include <OneWire.h>                 
#include <DallasTemperature.h>

#define Pin 5
String response;
int statusCode = 0;

OneWire ourWire(Pin);
DallasTemperature sensors(&ourWire);

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "v11F59BDA4C06A0B"; //device ID on Pushingbox for our Scenario

WiFiClient wifi;
HttpClient client = HttpClient(wifi, WEBSITE, 80);

const char* MY_SSID = "PMB";
const char* MY_PWD =  "holapaula";

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(MY_SSID);
    //Connect to WPA/WPA2 network.Change this line if using open/WEP network
    status = WiFi.begin(MY_SSID, MY_PWD);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
}



void loop() {

  // Wait between measurements.
  delay(10000);

  sensors.requestTemperatures();
    
  int celData = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(celData);

  Serial.println("\nSending Data to Server...");
  
  if (client.connect(WEBSITE, 80))
  {
    client.print("GET /pushingbox?devid=" + devid
                 + "&celData=" + (String) celData
                );

    // HTTP 1.1 provides a persistent connection, allowing batched requests
    // or pipelined to an output buffer
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(WEBSITE);
    client.println("User-Agent: MKR1000/1.0");
    //for MKR1000, unlike esp8266, do not close connection
    client.println();
    Serial.println("\nData Sent");

  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

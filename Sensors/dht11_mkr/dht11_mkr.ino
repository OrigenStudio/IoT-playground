#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <SPI.h>

#define DHTPIN 5    // what pin we're connected to, pin1 is 5th pin from end

// Uncomment whatever DHT sensor type you're using!
#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT21  // DHT 21
//#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN, DHTTYPE);
String response;
int statusCode = 0;

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vD37308811750B13"; //device ID on Pushingbox for our Scenario

WiFiClient wifi;
HttpClient client = HttpClient(wifi, WEBSITE, 80);

const char* MY_SSID = "PMB";
const char* MY_PWD =  "holapaula";


int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // check for the presence of the shield:
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

  //prefer to use float, but package size or float conversion isnt working
  //will revise in future with a string fuction or float conversion function

  int humidityData = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int celData = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int fehrData = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidityData) || isnan(celData) || isnan(fehrData))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  int hifData = dht.computeHeatIndex(fehrData, humidityData);
  // Compute heat index in Celsius (isFahreheit = false)
  int hicData = dht.computeHeatIndex(celData, humidityData, false);

  Serial.print("Humidity: ");
  Serial.print(humidityData);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(celData);
  Serial.print(" *C ");
  Serial.print(fehrData);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hicData);
  Serial.print(" *C ");
  Serial.print(hifData);
  Serial.println(" *F\n");

  Serial.println("\nSending Data to Server...");
  // if you get a connection, report back via serial:
   //Instantiate WiFi object, can scope from here or Globally

  //API service using WiFi Client through PushingBox then relayed to Google
  if (client.connect(WEBSITE, 80))
  {
    client.print("GET /pushingbox?devid=" + devid
                 + "&humidityData=" + (String) humidityData
                 + "&celData="      + (String) celData
                 + "&fehrData="     + (String) fehrData
                 + "&hicData="      + (String) hicData
                 + "&hifData="      + (String) hifData
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

#include <SD.h>
#include <SPI.h>
#define Pin 5

const int chipSelect = 4;
char cadena[555];
int i=0;

File myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card…");
    //pinMode(10, OUTPUT);
    //iniot SD card
   if (!SD.begin(chipSelect))
    {
   Serial.println("Card failed, or not present");
    return;
    }
   Serial.println("card initialized.");

myFile = SD.open("test.txt", FILE_WRITE);
  //open file for reading
   myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test2.txt");
  }
}



void loop() {
 
  File myFile = SD.open("test.txt", FILE_WRITE);

  //Serial.print("Writing to test.txt...");
  
  // if the file opened okay, write to it:
  if (myFile) {
    int value = analogRead(Pin);
    myFile.println(value);
    Serial.println(value);
    myFile.close();
    delay(3000);
  } 
  
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

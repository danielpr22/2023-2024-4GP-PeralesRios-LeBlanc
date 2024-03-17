#include <SoftwareSerial.h>
#define rxPin 11 // Broche 11 en tant que RX, à raccorder sur TX du HC-05
#define txPin 10 // Broche 10 en tant que TX, à raccorder sur RX du HC-05
#define baudrate 38400
SoftwareSerial mySerial(rxPin, txPin) // Définition du software serial

void setup(){
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    mySerial.begin(baudrate); 
    Serial.begin(baudrate); 
}

void loop() {
    int i = 0; 
    char someChar[32] = {0}; 

    while (Serial.available()) {    // We read the data from the serial port as soon as there is new
                                    // data available
        do {
            someChar[i++] = Serial.read();
            delay(3); 
        } while (Serial.available > 0)
        mySerial.println(someChar); 
        Serial.println(someChar);         
    }
    while (mySerial.available()) {
        Serial.print((char)mySerial.read()); 
    }
}
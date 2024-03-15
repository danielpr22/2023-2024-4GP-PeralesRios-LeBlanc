/*
  This example code is in the public domain.
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial

  Modified by Daniel PERALES RIOS and Maël LE BLANC, both students at INSA Toulouse.
*/

const int flexPin = A1;      // pin connected to voltage divider output

const float VCC = 5.0;      // voltage at Ardunio 5V line
const float R_DIV = 56000.0;  // resistor used to create a voltage divider
const float flatResistance = 31000.0; // resistance when flat
const float bendResistance = 67000.0;  // resistance at 90 deg bending

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(flexPin, INPUT);
}

void loop() {
  // Read the ADC, and calculate voltage and resistance from it
  int ADCflex = analogRead(flexPin);
  Serial.println("ADCflex: " + String(ADCflex));
  float Vflex = (ADCflex / 1024.0) * VCC;
  Serial.println("Vflex: " + String(Vflex) + "V");
  float Rflex = R_DIV * (VCC / Vflex - 1.0);
  Serial.println("Resistance: " + String(Rflex) + " ohms");

  // Use the calculated resistance to estimate the sensor's bend angle:
  float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);
  Serial.println("Bend: " + String(angle) + " degrees");
  Serial.println();

  delay(500);  // delay in between reads for stability
}

/*  Code from the Moodle slides
    Modified by Daniel PERALES RIOS and Maël LE BLANC
*/

#define encoderPinA 2 // CLK Output A
#define encoderPinB 4 // DT Output B
#define Switch 5 // Switch connection

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastDebounceTime = 0;
long debounceDelay = 80;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP); // Use internal pull-up resistors
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(encoderPinB), updateEncoder, CHANGE);
  Serial.begin(9600);
}

void loop() {
  Serial.print("Position: ");
  Serial.println(encoderValue);
}

void updateEncoder() {
  noInterrupts(); // Disable interrupts temporarily
  int MSB = digitalRead(encoderPinA);
  int LSB = digitalRead(encoderPinB);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--;
  }

  // Debouncing logic
  long currentTime = millis();
  if (currentTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentTime;
    lastEncoded = encoded;
  }
  interrupts(); // Re-enable interrupts
}

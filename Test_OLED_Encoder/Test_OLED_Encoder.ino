/*
  Contrôle d'un écran OLED
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.07.2021

  Modifié par Daniel PERALES RIOS et Maël LE BLANC, étudiants de l'INSA Toulouse
*/

#include <Adafruit_SSD1306.h>

// ***ENCODER***
//----------------------------------------------
#define encoderPinA 2 // CLK Output A
#define encoderPinB 4 // DT Output B
#define Switch 5 // Switch connection

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastEncoderValue = 0;

long lastDebounceTime = 0;
long debounceDelay = 120;
//----------------------------------------------

//***OLED***
//----------------------------------------------
#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// Define menu items
String menuItems[] = {"BT Options", "Flex Sensor", "Graphite Sensor", "Settings"};
int selectedItem = 0;
//----------------------------------------------

void setup() {
  Serial.begin(9600);

  //***ENCODER***
  //----------------------------------------------
  pinMode(encoderPinA, INPUT_PULLUP); // Use internal pull-up resistors
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  //----------------------------------------------

  //***OLED***
  //----------------------------------------------
  if (!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while (1); // Program stops (infinite loop) if something goes wrong here

  ecranOLED.clearDisplay();
  ecranOLED.setTextColor(SSD1306_WHITE);
  //----------------------------------------------
}

void loop() {
  displayMenu();
  handleMenuNavigation();
  encoderValue = abs(encoderValue % 4); 
  Serial.println(encoderValue);
  if ((encoderValue - lastEncoderValue) > 0) { // if we want to select the next element in the menu
    if (selectedItem < 3) {
      selectedItem++;
    }
  }
  else if (encoderValue - lastEncoderValue < 0) { // if we want to select the previous element in the menu
    if (selectedItem > 0) {
      selectedItem--;
    }
  }
  lastEncoderValue = encoderValue; 
  Serial.println("Selected:" + String(selectedItem));
  delay(200);
}

void displayMenu() {
  ecranOLED.clearDisplay();
  for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
    if (i == selectedItem) {
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlight selected item
    } else {
      ecranOLED.setTextColor(SSD1306_WHITE); // Regular color for other items
    }
    ecranOLED.setCursor(0, i * 10); // Adjust position for each item
    ecranOLED.print(menuItems[i]);
  }
  ecranOLED.display();
}

void handleMenuNavigation() {
  // Implement code to handle encoder rotation and button press to navigate the menu
}

// Example function to handle menu item selection
void handleMenuItemSelection(int selectedItem) {
  // Implement actions for each menu item
  switch (selectedItem) {
    case 0:
      // Action for Item 1
      break;
    case 1:
      // Action for Item 2
      break;
    case 2:
      // Action for Item 3
      break;
    case 3:
      // Action for Item 4
      break;
    default:
      break;
  }
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

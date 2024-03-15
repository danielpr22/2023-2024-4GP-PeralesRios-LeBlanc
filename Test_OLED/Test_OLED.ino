/*
  Contrôle d'un écran OLED
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.07.2021

  Modifié par Daniel PERALES RIOS et Maël LE BLANC, étudiants de l'INSA Toulouse
*/

#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// Define menu items
String menuItems[] = {"BT Options", "Flex Sensor", "Graphite Sensor", "Settings"};
int selectedItem = 0;

void setup() {
  Serial.begin(9600);

  if (!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while (1); // Program stops (infinite loop) if something goes wrong here

  ecranOLED.clearDisplay();
  ecranOLED.setTextColor(SSD1306_WHITE);
}

void loop() {
  displayMenu();
  handleMenuNavigation();
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

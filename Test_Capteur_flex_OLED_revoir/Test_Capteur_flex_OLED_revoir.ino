/*
  PARTIE ECRAN OLED
  Fichier :       afficherTexteSsd1306.ino
  Description :   Affiche du texte de test, sur un écran OLED i2c de 128x64 pixels,
                  fonctionnant avec un contrôleur SSD1306
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.07.2021
*/

#include <Adafruit_SSD1306.h>
#include <string.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
#define pinArduinoRaccordementSignalSW  2       // La pin D2 de l'Arduino recevra la ligne SW du module KY-040
#define pinArduinoRaccordementSignalCLK 3       // La pin D3 de l'Arduino recevra la ligne CLK du module KY-040
#define pinArduinoRaccordementSignalDT  4       // La pin D4 de l'Arduino recevra la ligne DT du module KY-040

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// Variables encodeur
int etatPrecedentLigneSW;           // Cette variable nous permettra de stocker le dernier état de la ligne SW, afin de le comparer à l'actuel
int etatPrecedentLigneCLK;          // Cette variable nous permettra de stocker le dernier état de la ligne CLK, afin de le comparer à l'actuel
int etatPrecedentLigneDT;           // Cette variable nous permettra de stocker le dernier état de la ligne DT, afin de le comparer à l'actuel

// Position du curseur
int x_pos = 0; 
int y_pos = 0; 


int compteur = 0; 
int ancien_compteur = 0; 

volatile unsigned int encoder0Pos = 0;
const int Vcc = 5;
const int R1 = 100000;
const int R2 = 1000;
const int R3 = 100000;
const int R5 = 10000;
int res = 0;
const int flexPin = A1;      // Pin connected to voltage divider output
const float R_DIV = 22000.0;  // resistor used to create a voltage divider
const float flatResistance = 26800.0; // resistance when flat
const float bendResistance = 55000.0;  // resistance at 90 deg

void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(flexPin, INPUT);

  //ENCODER : Initialization
  pinMode(pinArduinoRaccordementSignalSW, INPUT);
  pinMode(pinArduinoRaccordementSignalCLK, INPUT);
  pinMode(pinArduinoRaccordementSignalDT, INPUT);


  // Petite pause pour laisser le temps aux signaux de se stabiliser
  delay(200);

  // Mémorisation des valeurs initiales des lignes SW/CLK/DT, au démarrage du programme
  etatPrecedentLigneSW  = digitalRead(pinArduinoRaccordementSignalSW);
  etatPrecedentLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
  etatPrecedentLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);

  // Activation d'interruptions sur les lignes CLK et DT
  attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalCLK), changementSurLigneCLK, CHANGE);   // CHANGE => détecte tout changement d'état
  attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalSW), changementSurLigneSW, CHANGE);


  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche

  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
  ecranOLED.setTextColor(SSD1306_WHITE);                  // Affichage du texte en "blanc" (avec la couleur principale, en fait, car l'écran monochrome peut être coloré)
  ecranOLED.setTextSize(1.5);
}


void loop() {
  // GRAPHITE SENSOR: read the input on analog pin 0:
  float sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  // We convert the sensor value from digital to analog value
  float Value = sensorValue * Vcc / 1024.0;
  Serial.println(Value);

  // We convert the new analog sensor value into resistance
  res = ((Vcc / Value) * R1 * (1 + (R3/R2))) - (R1 + R5);

  // FLEX SENSOR: Read the ADC, and calculate voltage and resistance from it
  int ADCflex = analogRead(flexPin);
  float Vflex = ADCflex * Vcc / 1024.0;
  float Rflex = R_DIV * (Vcc / Vflex - 1.0);
  Serial.println("Flex Sensor: " + String(Rflex) + " ohms");

  // Use the calculated resistance to estimate the sensor's bend angle:
  float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);

  // print out the value you read:
  Serial.println("Graphite: " + String(res) + " ohms"); // We print the value in the serial to check
  Serial.println(" ");


  if ((compteur <= 0) && ((ancien_compteur - compteur) != 0)){
    ecranOLED.clearDisplay();
    ecranOLED.println("Calibration Potentiometre");
    ecranOLED.display(); 
  
  
  }
  else if ((compteur == 1) && (ancien_compteur - compteur != 0)){
    ecranOLED.clearDisplay();
    ecranOLED.println("Adresse BT");
    ecranOLED.display(); 
  }
  else if ((compteur == 2) && (ancien_compteur - compteur != 0)){
    ecranOLED.clearDisplay();
    ecranOLED.println("Mesures Resistances");
    ecranOLED.setCursor (0,0);
    ecranOLED.display(); 
  }
  else if ((compteur >= 3) && (ancien_compteur - compteur != 0)){
    ecranOLED.clearDisplay();
    ecranOLED.println("Mesures Tensions");
    ecranOLED.display(); 
  }
 
}

void changementSurLigneCLK() {

    // Lecture des lignes CLK et DT, issue du KY-040, arrivant sur l'arduino
    int etatActuelDeLaLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
    int etatActuelDeLaLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);
    ancien_compteur = compteur;

    // ************************
    // * CAS : Incrémentation *
    // ************************
    // Si CLK = 1 et DT = 0, et que l'ancienCLK = 0 et ancienDT = 1, alors le bouton a été tourné d'un cran vers la droite (sens horaire, donc incrémentation)
    if((etatActuelDeLaLigneCLK == HIGH) && (etatActuelDeLaLigneDT == LOW) && (etatPrecedentLigneCLK == LOW) && (etatPrecedentLigneDT == HIGH)) {

        // Alors on incrémente le compteur
        if (compteur == 3){
          compteur = 3;
        }
        else {
        compteur++;
        }

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = horaire | Valeur du compteur = "));
        Serial.println(compteur);
        
    }

    // ************************
    // * CAS : Décrémentation *
    // ************************
    // Si CLK = 1 et DT = 1, et que l'ancienCLK = 0 et ancienDT = 0, alors le bouton a été tourné d'un cran vers la gauche (sens antihoraire, donc décrémentation)
    if((etatActuelDeLaLigneCLK == HIGH) && (etatActuelDeLaLigneDT == HIGH) && (etatPrecedentLigneCLK == LOW) && (etatPrecedentLigneDT == LOW)) {

        // Alors on décrémente le compteur
        if (compteur == 0){
          compteur = 0 ;
        }
        else {
          compteur--;
        }
        
        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
        Serial.println(compteur);
        
    }

    // Et on mémorise ces états actuels comme étant "les nouveaux anciens", pour le "tour suivant" !
    etatPrecedentLigneCLK = etatActuelDeLaLigneCLK;
    etatPrecedentLigneDT = etatActuelDeLaLigneDT;
    
    
}


// =============================================
// Routine d'interruption : changementSurLigneSW
// =============================================
void changementSurLigneSW() {
  
    // On lit le nouvel état de la ligne SW
    int etatActuelDeLaLigneSW = digitalRead(pinArduinoRaccordementSignalSW);
    if(etatActuelDeLaLigneSW == LOW) delay(10);

    // On affiche le nouvel état de SW sur le moniteur série de l'IDE Arduino
    if(etatActuelDeLaLigneSW == LOW)
        Serial.println(F("Bouton SW appuyé"));
    else
        Serial.println(F("Bouton SW relâché"));

    // Puis on mémorise le nouvel état de la ligne SW
    etatPrecedentLigneSW = etatActuelDeLaLigneSW;

}

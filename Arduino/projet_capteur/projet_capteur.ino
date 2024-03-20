#include <Adafruit_SSD1306.h>
#include <String.h>
#include <SoftwareSerial.h>

#define nombreDePixelsEnLargeur 128  // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64   // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED -1           // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED 0x3C     // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
#define encoder0PinA 2               //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB 4               //DT Output B
#define PinCapteur A0
#define PinFlex A1

#define rxPin 7 //Bluetooth
#define txPin 8 //Bluetooth
#define baudrate 38400
SoftwareSerial mySerial(rxPin, txPin);


Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
int tailleDeCaractere = 2;
volatile unsigned int Encodeur = 0;


void setup() {
  Serial.begin(9600);

  pinMode(PinCapteur, INPUT);  //Pin relié au capteur
  pinMode(PinFlex, INPUT);     //Pin relié au Flex sensor

  pinMode(encoder0PinA, INPUT);  //Pin relié à l'encodeur : CLK
  pinMode(encoder0PinB, INPUT);  //Pin relié à l'encodeur : DT

  /*
  pinMode(5, INPUT); //Pin relié à l'encodeur : SW
  
  pinMode(10, INPUT); //Pin relié au Potentiomètre MCP41100 -CS 
  pinMode(11, OUTPUT); //Pin relié au MCP41100 SI
  pinMode(13, OUTPUT); //Pin relié au MCP41100 SCK*/

  // Initialisation de l'écran OLED
  if (!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) {
    Serial.print("échec d'initialisation");
    while (1)
      ;
  }  // Arrêt du programme (boucle infinie) si échec d'initialisation

  //Initialisation Bluetooth
  pinMode(txPin,OUTPUT); 
  pinMode(rxPin,INPUT);


  // initialisation utile pour l'encodeur rotatoire
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  attachInterrupt(0, doEncoder, RISING);  // encoder pin on interrupt 0 - pin2
}



void Affichage_OLED(String entete, int Pin) {

  ecranOLED.clearDisplay();  // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(tailleDeCaractere);
  ecranOLED.setTextColor(SSD1306_WHITE);  //Couleur du texte : blanc
  ecranOLED.setCursor(0, 0);              // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
  ecranOLED.display();                    //permet d'effacer l'affichage précédent

  String texte = String(analogRead(Pin));           //texte reçoit les données transmises par le capteur graphite sous forme de String
  String a_texte_capteur = String(entete + texte);  //on concatène les deux chaînes pour avoir un bel affichage

  // /*Serial.print("affichage de texte :");  */
  // Serial.println(texte);
  //  Serial.print("analog read retourne :");
  //Serial.print(analogRead(A0)); */

  ecranOLED.print(a_texte_capteur);  // Affichage du texte en "blanc" (avec la couleur principale, en fait, car l'écran monochrome peut être coloré)


  ecranOLED.display();  // Transfert le buffer à l'écran
  delay(200);


}


int usebyte;
int i;

void EnvoiBT (int Pin) //Pin est le pin
{
// put your main code here, to run repeatedly:
  usebyte = mySerial.read();
  while (usebyte!=3)
 {
  Serial.println(analogRead(Pin));
   mySerial.write(analogRead(Pin));
   usebyte = mySerial.read();
 }
  


}

String texte_capteur = "Capteur Graphite : ";
String texte_flex = "Capteur Flex : ";

void loop() {



  if (Encodeur % 2) {  //boucle permettant de naviguer dans le menu : si l'encodeur est sur une position paire, il affiche le capteur flex, et s'il est sur une position impaire, il affiche le capteur graphite.
    Affichage_OLED(texte_flex, PinFlex);

  } else {
    Affichage_OLED(texte_capteur, PinCapteur);
  }
  if (mySerial.read()==1) {
    EnvoiBT(PinCapteur);
  }
  if (mySerial.read()==2) {
    EnvoiBT(PinFlex);
  }
}

//définition de la fonction d'interruption. Elle est exécutée dès que la pin 2 est sur HIGH, c'est-à-dire dès que l'on tourne l'encodeur.
//à voir : peut-on supprime la partie où on fait Encodeur--? On pourrait seulement augmenter le chiffre..?
void doEncoder() {
  Serial.println("je suis dans doEncoder");
  if (digitalRead(encoder0PinB) == HIGH) {
    Encodeur++;
  } else {
    Encodeur--;
  }
}










 
   

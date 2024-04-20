#include <Adafruit_SSD1306.h>
#include <String.h>
#include <SoftwareSerial.h>
#include <WString.h>
#include <SPI.h>

#define nombreDePixelsEnLargeur 128  // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64   // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED -1           // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED 0x3C     // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
#define encoder0PinA 2               //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB 4               //DT Output B
#define PinCapteur A0
#define PinFlex A1
#define CsPin 10 // pin du Chip Select du MCP

#define rxPin 7 //Bluetooth
#define txPin 8 //Bluetooth
#define baudrate 9600

float R1= 100000.0;
float R3 = 100000.0;
float R6 = 100000.0;
float R2 = 1000.0;
float R5 = 10000.0;
float calibre=0.000001;//en MOhm
float Vcc=5.0;
float Voltage =0.0;
float Resistance = 0.0;

SoftwareSerial HC05(rxPin, txPin);

//relatif au MCP

const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 92500;   // 100k pot resistance between terminals A and B, 
                                      // mais pour ajuster au multimètre, je mets 92500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
int tailleDeCaractere = 2;
volatile unsigned int Encodeur = 0;

void setup() {
  Serial.begin (baudrate);

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
  HC05.begin(9600);


  // initialisation utile pour l'encodeur rotatoire
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  attachInterrupt(0, doEncoder, RISING);  // encoder pin on interrupt 0 - pin2

  //initialisation MCP

  digitalWrite(CsPin, HIGH);        // chip select default to de-selected
  pinMode(CsPin, OUTPUT);           // configure chip select as output
  SPI.begin();
}

void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(CsPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(CsPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  /*Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance wiper to B terminal: ");
  Serial.print(resistanceWB);
  Serial.println(" ohms");*/
}


void Affichage_OLED() {

  ecranOLED.clearDisplay();  // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(tailleDeCaractere);
  ecranOLED.setTextColor(SSD1306_WHITE);  //Couleur du texte : blanc
  ecranOLED.setCursor(0, 0);              // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
  ecranOLED.display();                    //permet d'effacer l'affichage précédent


if (Encodeur % 2) {  //boucle permettant de naviguer dans le menu : si l'encodeur est sur une position paire, il affiche le capteur flex, et s'il est sur une position impaire, il affiche le capteur graphite.
     float R = -0.2636*analogRead(PinFlex)+185.9;
     int Angle=((R-30)*2);
      String texte = String(analogRead(PinFlex));

ecranOLED.println(F("Capteur Flex :"));
ecranOLED.print(F("R :"));
ecranOLED.println(R);
ecranOLED.print(F("Angle :"));
ecranOLED.print(Angle);
ecranOLED.display();

  } else {

    Voltage=analogRead(PinCapteur)*5.0/1024.0;
    Resistance=((R1*((R2+R3)/R2)*(Vcc/Voltage))-R5-R1)*calibre;
    String texte = String(Resistance);

    ecranOLED.print(F("Capteur Graphite :    "));
    ecranOLED.print(texte);
    ecranOLED.display();

  }
delay(200);
}


int usebyte;
int i;

void EnvoiBT (int Pin) //Pin est le pin
{
// put your main code here, to run repeatedly:
  Serial.println("Je BT");   //Pour vérif qu'on est dans la boucle
  usebyte = HC05.read();
  if (usebyte!=3)
  {
    int Mes = analogRead(Pin);
  Serial.println(Mes);
   char Val[10];
   dtostrf(Mes, 5, 2, Val);
   HC05.write(Val);
   usebyte = HC05.read();
   Serial.println("fini");
   delay(500);
  
 //}
 }
  }



void loop() {
  setPotWiper(pot0, R2);  
    Affichage_OLED();
  while (HC05.available()) {
    int Recu = HC05.read();



    while (Recu==1)
      {
        EnvoiBT(PinCapteur);
        
      }
      while (Recu==2)
      {
        EnvoiBT(PinFlex);
        
      }

  }

}

//définition de la fonction d'interruption. Elle est exécutée dès que la pin 2 est sur HIGH, c'est-à-dire dès que l'on tourne l'encodeur.
//à voir : peut-on supprime la partie où on fait Encodeur--? On pourrait seulement augmenter le chiffre..?
void doEncoder() {
  if (digitalRead(encoder0PinB) == HIGH) {
    Encodeur++;
  } else {
    Encodeur--;
  }
}
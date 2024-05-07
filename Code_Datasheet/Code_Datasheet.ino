#include <Adafruit_SSD1306.h>
#include <String.h>
#include <SoftwareSerial.h>
#include <WString.h>
#include <SPI.h>

#define PinCapteur A0
#define PinFlex A1
#define CsPin 10 // pin du Chip Select du 
#define encoder0PinA 2               //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB 4 
#define baudrate 38400
float R1=100000.0;
float R3=100000.0;
float R6=100000.0;

float R2 ;
float R5 = 10000.0;
float calibre=0.000001;//en MOhm
float Vcc=5.0;
float Voltage ;
float Resistance;

volatile unsigned int Encodeur = 0;
//relatif au MCP


const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 92500;   // 100k pot resistance between terminals A and B, 
                                      // mais pour ajuster au multimètre, je mets 92500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001



void setup() {

  Serial.begin(9600);
  pinMode(PinCapteur, INPUT);  //Pin relié au capteur

   pinMode(10, INPUT); //Pin relié au Potentiomètre MCP41100 -CS 
  pinMode(11, OUTPUT); //Pin relié au MCP41100 SI
  pinMode(13, OUTPUT); //Pin relié au MCP41100 SCK


  //initialisation MCP

  digitalWrite(CsPin, HIGH);        // chip select default to de-selected
  pinMode(CsPin, OUTPUT);           // configure chip select as output
  SPI.begin();

setPotWiper(pot0,25);

}

//programme du MCP

void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(CsPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(CsPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  R2 = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance wiper to B terminal: ");
  Serial.print(R2);
  Serial.println(" ohms");
}



void loop() {
Voltage=analogRead(PinCapteur)*5.0/1024.0;
//Serial.println(Voltage);
Resistance=((R1*((R2+R3)/R2)*(Vcc/Voltage))-R5-R1);//en ohms
Resistance=Resistance*calibre;//en MOhms
//Serial.println(Resistance);
delay(200);
}


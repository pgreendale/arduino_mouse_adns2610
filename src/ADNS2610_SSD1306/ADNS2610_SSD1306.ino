/*
* Based on CT Article "Mauskamera" (Daniel Bachfeld)
* with ADNS-2610
*/
#define SCLK 2
#define SDIO 3

#define REG_CONFIGURATION 0x00
#define REG_REVISION_ID 0x01
#define REG_DELTA_X 0x02
#define REG_DELTA_Y 0x03
#define REG_SQUAL 0x04
#define REG_MAXIMUM_PIXEL 0x05
#define REG_MINIMUM_PIXEL 0x05
#define REG_PIXEL_SUM 0x07
#define REG_PIXEL_DATA 0x08
#define REG_SHUTTER_UPPER 0x09
#define REG_SHUTTER_LOWER 0

byte frame[324];
byte i;
byte camWidth = 18;
byte camHeight = 18;
const byte maskNoSleep  = 0x01;
const byte maskPID      = 0xE0;

/*
* Display Configuration, based on Adafruit tutorial
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   8
#define LOGO_WIDTH    8
static const unsigned char PROGMEM logo_bmp[] =
{ 0b01111110,
  0b00011000,
  0b10011001,
  0b10011001,
  0b01011010,
  0b01011010, 
  0b00111100,
  0b00011000
};

void setup() {
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT); 
  Serial.begin(38400);
  Serial.flush(); 

  // Mouse configuration 
   
  byte config = readRegister(REG_CONFIGURATION); 
  config |= B00000001; // // Maus immer an, LED immer an
  delayMicroseconds(1);
  writeRegister(REG_CONFIGURATION, config);
  Serial.write("Mouse Ready"); 
  //Display Configuration 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(500);
  // display.display() is NOT necessary after every single drawing command,
  testdrawbitmap();    // Draw a small bitmap image
}

void loop() {
  getFrame(frame);
  displayFrame(frame); 
  //delay(1); 
}

void testdrawbitmap(void) {
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(500);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

/*
* Mouse Functions 
*/
byte readRegister(byte address) {
  pinMode (SDIO, OUTPUT);

  for (byte i=128; i >0 ; i >>= 1) {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, (address & i) != 0 ? HIGH : LOW);
    digitalWrite (SCLK, HIGH);
  }
  pinMode (SDIO, INPUT);
  delayMicroseconds(1); // tHOLD = 100us min.
  byte res = 0;
  for (byte i=128; i >0 ; i >>= 1) {
    digitalWrite (SCLK, LOW);
    digitalWrite (SCLK, HIGH);
    if( digitalRead (SDIO) == HIGH )
      res |= i;
  }

  return res;
}


void writeRegister(byte address, byte data) {
  address |= 0x80; 
  pinMode (SDIO, OUTPUT);

  for (byte i = 128; i > 0 ; i >>= 1) {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, (address & i) != 0 ? HIGH : LOW);
    digitalWrite (SCLK, HIGH);
  }

  for (byte i = 128; i > 0 ; i >>= 1) {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, (data & i) != 0 ? HIGH : LOW);
    digitalWrite (SCLK, HIGH);
  }

  delayMicroseconds(100); // tSWW, tSWR = 100us min.
}

void getFrame(byte* pixel){
  writeRegister(REG_PIXEL_DATA, 0xAA); //irgendwas in das Register schreiben, um das Senden zu starten
  delayMicroseconds(500);
  int count = 0;
  do {
    byte data = readRegister(REG_PIXEL_DATA);
      frame[count++] = ((data & B00111111)); 
      delayMicroseconds(500);
   } 
  while (count != 324);  
} 

void displayFrame(byte* pixel){
  int xzoomfactor = 4 ;
  int yzoomfactor = 4 ; 
  display.clearDisplay();
  for(int i = 0; i < camHeight; i++) {
    
    for(int j = 0; j < camWidth; j++) {
        byte pix = (frame[(i*18) + j]);
        for(int xzoom = 0; xzoom < xzoomfactor ; xzoom ++){
          for(int yzoom = 0; yzoom < yzoomfactor ; yzoom ++){
          display.drawPixel((display.width()-camWidth*xzoomfactor)/2 + i*xzoomfactor  + xzoom,
                           (display.height()+camHeight*yzoomfactor)/2 - j*yzoomfactor + yzoom,
                           (pix >= 24)
                           );
         }
        }
      }
  }
  display.display();
}

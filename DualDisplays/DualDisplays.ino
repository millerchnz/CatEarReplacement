
// ESP32_two_240x280_ST7789
//
// images from https://www.pngwing.com/e
//
// found via Kitecraft at Github: https://github.com/kitecraft/Multiple_TFT_Displays/blob/master/Basic_Double_TFT/Basic_Double_TFT.ino 
// more at https://www.xtronical.com/multiplescreens/
//
// microcontroller: ESP32-WROOM32
// displays: two specimen 1.69 inch 3.3V TFT 240*280 pixels ST7789A controller
// simultaneous independent display of the sketch 
//
// Floris Wouterlood 
// December 4, 2023
// public domain
//
// all x and y coordinates relative to (center) coordinates x=120 and y=120
// is your screen bigger than 240x240 -- change the center coordinates 
//
// pin layout:
// both displays     ESP32
//     GND ---------- GND
//     VCC ---------- 3.3V
//     SCL ---------- D18 SCLK
//     SDA ---------- D23 MOSI
//     MISO---------- D22
//     MOSI---------- D21 
//     SLCK---------- D17
//     RES ---------- D4
//     DC  ---------- D2
//     BLK ---------- 3V3 - essential
//     SDCS --------- 34
//
//  CS of screen 0 to D21, CS of screen 1 to D5 - right and left if pins on breakout 'north' of display 
#define MAX_IMAGE_WIDTH 240 // Adjust for your images
#define MAX_IMAGE_HEIGHT 230 // Adjust for your images

#include "FS.h"
#include "SD.h"
#include "User_Setup.h" 
#include <SPI.h>
#include <TFT_eSPI.h> 


   // Include the PNG decoder library
#include <PNGdec.h>

// do not forget to use User_Setup_xxxx.h that matches micr ocontroller-display combination

   #define screen_0_CS 27 //21              
   #define screen_1_CS  33                                         // each display has its own CS pin
   #define SD_CS  16
   #define SCLK  21
   #define MISO  22
   #define MOSI  17
   TFT_eSPI tft = TFT_eSPI();   
   TFT_eSprite img = TFT_eSprite(&tft);
   //TFT_eSprite pngSprite = TFT_eSprite(&tft);
    

// some other principal colors
// RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
   #define WHITE       0xFFFF
   #define BLACK       0x0000
   #define BLUE        0x001F
   #define RED         0xF800
   #define GREEN       0x07E0
   #define CYAN        0x07FF
   #define MAGENTA     0xF81F
   #define YELLOW      0xFFE0
   #define GREY        0x2108 
   #define ORANGE      0xFBE0
   #define TEXT_COLOR  0xFFFF                                     
   #define AFRICA      0xAB21
   #define BORDEAUX    0xA000
   #define VOLTMETER   0xF6D3 

   int pin;
   int displayChoice;
   int iteration=1;
   int j;
int16_t xpos = 0;
int16_t ypos = 0;

File pngfile;

PNG png;

int pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  //png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  //tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);

  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  img.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);

  return 1;
}
void * pngOpen(const char *filename, int32_t *size) {
  Serial.printf("Attempting to open %s\n", filename);
  pngfile = SD.open(filename, "r");
  *size = pngfile.size();
  return &pngfile;
}

void pngClose(void *handle) {
  //File pngfile = *((File*)handle);
  if (pngfile) pngfile.close();
}

int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length) {
  if (!pngfile) return 0;
  //page = page; // Avoid warning
  return pngfile.read(buffer, length);
}

int32_t pngSeek(PNGFILE *page, int32_t position) {
  if (!pngfile) return 0;
  //page = page; // Avoid warning
  return pngfile.seek(position);
}
int* loadImageSprite(char *name) {
      int16_t rc = png.open(name, pngOpen, pngClose, pngRead, pngSeek, pngDraw);  
      int* arr = (int*)malloc(2* sizeof(int));
      img.fillSprite(TFT_BLACK);
      if (rc == PNG_SUCCESS) {
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
         arr[0] = png.getWidth();arr[1] = png.getHeight();
        if (png.getWidth() > MAX_IMAGE_WIDTH) {
          Serial.println("Image too wide for allocated line buffer size!");
        }
        else {
          rc = png.decode(NULL, 0);
          png.close();
        }
      }
      return arr;
}

void drawImageSprite(int cs,int xpos, int ypos) {
   digitalWrite (cs,LOW);
   img.pushSprite(xpos, ypos);
   digitalWrite (cs,HIGH);  
}
void heartpng() {
  loadImageSprite("/heart.png");
  drawImageSprite(screen_1_CS,0,10);
  drawImageSprite(screen_0_CS,0,10);
}

void firework() {
  loadImageSprite("/firework.png");
  drawImageSprite(screen_1_CS,0,10);
  drawImageSprite(screen_0_CS,0,10);
}

void snake() {
  int* dim = loadImageSprite("/snake.png");
  drawImageSprite(screen_1_CS,(240-dim[1])/2,(240-dim[0])/2);
  drawImageSprite(screen_0_CS,(240-dim[1])/2,(240-dim[0])/2);
}

void karen() {
  int* dim = loadImageSprite("/karen.png");
  drawImageSprite(screen_1_CS,(240-dim[1])/2,(240-dim[0])/2);
  drawImageSprite(screen_0_CS,(240-dim[1])/2,(240-dim[0])/2);
  }
void cry() {
  int* dim = loadImageSprite("/cry.png");
  drawImageSprite(screen_1_CS,(240-dim[1])/2,(240-dim[0])/2);
  drawImageSprite(screen_0_CS,(240-dim[1])/2,(240-dim[0])/2);
}
void balloon() {
  int* dim = loadImageSprite("/balloon.png");
  drawImageSprite(screen_1_CS,(240-dim[0])/2,(240-dim[1])/2+20);
  drawImageSprite(screen_0_CS,(240-dim[0])/2,(240-dim[1])/2+20);
}
void setup() {

   Serial.begin (115200);   

   pinMode (screen_0_CS, OUTPUT);
   pinMode (screen_1_CS, OUTPUT);
   pinMode (SD_CS, OUTPUT);

   digitalWrite (screen_0_CS, LOW);                               // we need to 'init' both displays
   digitalWrite (screen_1_CS, LOW);                               // at the same time - so set both cs pins low 
   tft.init (); 
   tft.fillScreen(BLACK);
     
   digitalWrite (screen_0_CS, HIGH);                              // set both CS pins HIGH, or 'inactive'
   digitalWrite (screen_1_CS, HIGH);
   img.createSprite(240, 230);
   img.setRotation(0);
   img.fillSprite(TFT_BLACK);
  
   // all 'tft.' calls to either display must be done after first setting the appropriate cs pin low 
   //img.fillScreen (BORDEAUX); 
   //for (j=0; j<4; j++) img.drawCircle (120, 160, j+110, YELLOW); 
   img.setTextColor (WHITE,GREY);
   img.setCursor (60,80);
   img.setTextSize (3);
   img.setTextColor (WHITE, BORDEAUX); 
   img.print ("display");
   img.setCursor (105,155);
   img.setTextSize (7);
   img.print (iteration++);
   img.drawCircle (120,175,50,YELLOW);

   digitalWrite (screen_0_CS, LOW);                               // start updating this screen
   //tft.setRotation (0);  
   img.pushSprite(0, 0);
   digitalWrite (screen_0_CS, HIGH);                              // set the cs pin back to HIGH when finished with this particular display

   //img.fillScreen (AFRICA);
   //for (j=0; j<4; j++) img.drawCircle (120, 120, j+110, YELLOW); 
   img.setTextColor (WHITE,AFRICA);
   img.setCursor (60,80);
   img.setTextSize (3);
   img.print ("display");
   img.setCursor (105,155);
   img.setTextSize (7);
   img.print (iteration++);
   img.drawCircle (120,175,50,YELLOW);

   // update screen 1
   digitalWrite (screen_1_CS, LOW);
   //tft.setRotation (0);  
   img.pushSprite(0, 0);
   digitalWrite (screen_1_CS, HIGH);                              // close updating this screen

   delay(100);

   SPI.begin(SCLK, MISO, MOSI, SD_CS);
   if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  File file = SD.open("/test.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print("hello")) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();  
}



void heart() {
   digitalWrite (screen_0_CS,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (80,80,50,RED);
   img.fillSmoothCircle (160,80,50,RED);
   for(int i=0;i<100;i++) img.drawLine(31+i,100,120,200,RED);
   for(int i=0;i<100;i++) img.drawLine(209-i,100,120,200,RED);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   digitalWrite (screen_1_CS,LOW);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}

void eye() {
   digitalWrite (screen_0_CS,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,140,50,YELLOW);
   img.fillSmoothCircle (120,140,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   digitalWrite (screen_1_CS,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,140,50,YELLOW);
   img.fillSmoothCircle (120,140,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}


void righteye() {
   digitalWrite (screen_0_CS,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (210,120,50,YELLOW);
   img.fillSmoothCircle (210,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   digitalWrite (screen_1_CS,LOW);
   //img.fillSprite (BLACK);
   //img.fillSmoothCircle (40,120,50,YELLOW);
   //img.fillSmoothCircle (40,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}


void lefteye() {
   digitalWrite (screen_0_CS,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (30,120,50,CYAN);
   img.fillSmoothCircle (30,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   digitalWrite (screen_1_CS,LOW);
   //img.fillSprite (BLACK);
   //img.fillSmoothCircle (120,120,50,RED);
   //img.fillSmoothCircle (120,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}


void sheepisheye() {
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,150,50,YELLOW);
   img.fillSmoothCircle (120,150,20,BLACK);
   
   //for(int i=0;i<20;i++) img.drawLine(40,80+i,120,0+i,YELLOW);
   for(int i=0;i<20;i++) img.drawLine(120,0+i,200,80+i,YELLOW);
   
   digitalWrite (screen_0_CS,LOW);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,150,50,YELLOW);
   img.fillSmoothCircle (120,150,20,BLACK);
   
   for(int i=0;i<20;i++) img.drawLine(40,80+i,120,0+i,YELLOW);
   //for(int i=0;i<20;i++) img.drawLine(120,0+i,200,80+i,YELLOW);

   digitalWrite (screen_1_CS,LOW);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}

void angryeye() {
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,200,50,RED);
   img.fillSmoothCircle (120,200,20,BLACK);
   
   for(int i=0;i<40;i++) img.drawLine(40,200+i,200,0+i,RED);
  
   digitalWrite (screen_0_CS,LOW);
   img.pushSprite(0, 10);
   digitalWrite (screen_0_CS,HIGH);  

   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,200,50,RED);
   img.fillSmoothCircle (120,200,20,BLACK);
   
   for(int i=0;i<40;i++) img.drawLine(200,200+i,40,0+i,RED);

   digitalWrite (screen_1_CS,LOW);
   img.pushSprite(0, 10);
   digitalWrite (screen_1_CS,HIGH);  
}

void loop() {
   balloon();
   delay (500);
   snake();
   delay (500);
   karen();
   delay (500);
   cry();
   delay (500);
   heartpng();
   delay (500);
   firework();
   delay (500);  
   heart();
   //Serial.println("heart");
   delay (500);  
   eye();
   //Serial.println("eye");
   delay (500);  
   lefteye();
   //Serial.println("left");
   delay (500);  
   righteye();
   //Serial.println("right");
   delay (500);  
   sheepisheye();
   //Serial.println("sheepish");
   delay (500);  
   angryeye();
   //Serial.println("angry");
   delay (500);  
}

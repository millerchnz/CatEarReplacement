
//
// pin layout:
// both displays     ESP32
//     GND ---------- GND  (Shared between displays and SD card)
//     VCC ---------- 3.3V (Shared between displays and SD card)
//
//     TFT Display pins:
//     SCL  ---------- D18  (Shared between displays)
//     SDA  ---------- D23  (Shared between displays)
//     RES  ---------- D4   (Shared between displays)
//     DC   ---------- D2   (Shared between displays)
//     BLK  ---------- 3V3  (Shared between displays)
//     CS_0 ---------- 27   Display 0 Cable select
//     CS_1 ---------- 33   Display 1 Cable select
//
//     SD Card Pins:
//     MISO---------- D22
//     MOSI---------- D21 
//     SLCK---------- D17
//     SDCS --------- 34
//
// All x and y coordinates relative to (center) coordinates x=120 and y=120
// is your screen bigger than 240x240 -- change the center coordinates 
//
//  CS of screen 0 to D21, CS of screen 1 to D5 - right and left if pins on breakout 'north' of display 


//SD Card libraries
#include "FS.h"
#include "SD.h"

#include "draw.h"
#include <cstring> 
#include <esp_heap_caps.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//SD Card const
#define SD_CS    16
#define SCLK     21
#define MISO     22
#define MOSI     17

BluetoothSerial SerialBT;

void setup() {

   Serial.begin (115200); 
    size_t freePsram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    size_t totalPsram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);

    Serial.print("Free PSRAM: ");
    Serial.print(freePsram);
    Serial.println(" bytes");

    Serial.print("Total PSRAM: ");
    Serial.print(totalPsram);
    Serial.println(" bytes");  

  SerialBT.begin("ESP32DualDisplay"); //Bluetooth device name

   //init cable select pins
   pinMode (TFT_CS_0, OUTPUT);
   pinMode (TFT_CS_1, OUTPUT);
   pinMode (SD_CS, OUTPUT);

   setupDisplay();

  //init the SD card
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
}

int readCommand() {
  bool reading = true;
  int v = 0;
  while(char c = SerialBT.read()) {
    if(c == 13) {
      //read 10 to finish command
      SerialBT.read();
      return v;
      
    } else {
      v = v*10 + c - 48;
      if(v>999) {
        while(c=SerialBT.read()!=10);
        return -1;
      }
    }
  }
}

#define DELAY 1000
void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    
    int v = readCommand();
    switch(v) {
      case 0:
        SerialBT.println("No value");
        return;
      case -1:
        SerialBT.println("Command overflowed");
        return;
    }
    
    bool found = false;
    File root = SD.open("/");
    int fCnt = 1;
    while(File file = root.openNextFile()) {
      String strname = file.name();
      strname = "/"+strname;
      if(!file.isDirectory()&&(strname.endsWith(".png")||strname.endsWith(".PNG"))) {
        if(fCnt++==v) {
          found = true;
          SerialBT.printf("Image at %d is %s\n",v,strname);
          char s[strname.length()+1];
          strname.toCharArray(s,strname.length()+1);
          int* dim = loadImageSprite(s);
          drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
        }
      }
    }
    if(!found) {
      SerialBT.printf("Image at %d doesn't exist\n",v);
    }

  }
  delay(20);
  /*
  File root = SD.open("/");
  while(File file = root.openNextFile()) {
    String strname = file.name();
    strname = "/"+strname;
    if(!file.isDirectory()&&strname.endsWith(".png")) {
      char s[strname.length()+1];
      strname.toCharArray(s,strname.length()+1);
      int* dim = loadImageSprite(s);
      drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
      delay (DELAY);    
    }
  }
  

  int* dim = loadImageSprite("/angry1.PNG");
  drawImageSprite(TFT_CS_0,(240-dim[0])/2,(240-dim[1])/2);
  drawImageSpriteHorizontalMirror(TFT_CS_1,(240-dim[0])/2,(240-dim[1])/2);
  
  delay (DELAY);    
  delay (DELAY);
  */    
}

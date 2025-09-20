#include "draw.h"
#include "angry1.h"
#include "angry2.h"
#include "heart.h"
#include "balloon.h"
#include "cry.h"

//display driver for BOTH displays
TFT_eSPI tft = TFT_eSPI();

//Sprite for display framebuffer
TFT_eSprite img = TFT_eSprite(&tft);

//PNG display variables
File pngfile;
PNG png;
int16_t xpos, ypos;

void setupDisplay() {
  
   //Init displays
   digitalWrite (TFT_CS_0, LOW);
   digitalWrite (TFT_CS_1, LOW);
   tft.init (); 
   tft.fillScreen(BLACK);  
   digitalWrite (TFT_CS_0, HIGH);
   digitalWrite (TFT_CS_1, HIGH);

   //create the sprite
   img.setAttribute(PSRAM_ENABLE, true);
   img.createSprite(50, 25);
   img.setRotation(0);
   img.fillSprite(TFT_BLACK);

}

TFT_eSprite* getSprite(){return &img;}
TFT_eSPI* getTFT(){return &tft;}

int pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
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
  if (pngfile) pngfile.close();
}

int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length) {
  if (!pngfile) return 0;
  return pngfile.read(buffer, length);
}

int32_t pngSeek(PNGFILE *page, int32_t position) {
  if (!pngfile) return 0;
  return pngfile.seek(position);
}

int* loadImageSprite(const char *name) {
  img.fillSprite (BLACK);
  int16_t rc = png.open(name, pngOpen, pngClose, pngRead, pngSeek, pngDraw);  
  int* arr = (int*)malloc(2* sizeof(int));
  if (rc == PNG_SUCCESS) {
    Serial.printf("image specs: %s (%d x %d), %d bpp, pixel type: %d\n", name, png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
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
   tft.fillScreen(BLACK);
   img.pushSprite(xpos, ypos);
   digitalWrite (cs,HIGH);  
}

void drawImageSpriteDual(int xpos, int ypos) {
   digitalWrite (TFT_CS_1,LOW);
   digitalWrite (TFT_CS_0,LOW);
   tft.fillScreen(BLACK);
   img.pushSprite(xpos, ypos);
   digitalWrite (TFT_CS_1,HIGH);
   digitalWrite (TFT_CS_0,HIGH);
}
/*
 * This is 100ms faster end to end than loading a 20k file and rendering
 */
void drawImageSpriteHorizontalMirror(int cs,int xpos, int ypos) {
  int w=img.width(),h=img.height();
  for(int i=0;i<w/2;i++)
  for(int j=0;j<h;j++) {
    uint16_t t1 = img.readPixel(i,j), t2=img.readPixel(w-i,j);
    img.drawPixel(i,j,t2);
    img.drawPixel(w-i,j,t1);
  }
  drawImageSprite(cs,xpos,ypos);  
}

int pngDrawFLASH(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
  return 1;
}

int showImage(int disp,int img) {
  int16_t rc;
  switch(img) {
    case 1:
      return showImage(1,103)|showImage(2,104);
    case 2:
      rc = png.openFLASH((uint8_t *)balloon_png, sizeof(balloon_png), pngDrawFLASH);
      break;
    case 3:
      rc = png.openFLASH((uint8_t *)heart_png, sizeof(heart_png), pngDrawFLASH);
      break;
    case 4:
      rc = png.openFLASH((uint8_t *)cry_png, sizeof(cry_png), pngDrawFLASH);
      break;
      
    case 103:
      rc = png.openFLASH((uint8_t *)angry1_png, sizeof(angry1_png), pngDrawFLASH);
      break;
    case 104:
      rc = png.openFLASH((uint8_t *)angry2_png, sizeof(angry2_png), pngDrawFLASH);
      break;
    default: return -1;
  }

  if(disp&1) digitalWrite (TFT_CS_0,LOW); 
  if(disp&2) digitalWrite (TFT_CS_1,LOW);
  tft.fillScreen(BLACK);

  if (rc == PNG_SUCCESS) {
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    xpos = (240-png.getWidth())/2;
    ypos = (240-png.getHeight())/2;
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
  } else Serial.println("PNG load from flash failed");

  if(disp&1) digitalWrite (TFT_CS_0,HIGH);
  if(disp&2) digitalWrite (TFT_CS_1,HIGH);
  return 1;
}

void heartpng() {
  int* dim = loadImageSprite("/heart.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void firework() {
  int* dim = loadImageSprite("/firework.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void snake() {
  int* dim = loadImageSprite("/snake.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void karen() {
  int* dim = loadImageSprite("/karen.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void arrow() {
  int* dim = loadImageSprite("/arrow.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void cry() {
  int* dim = loadImageSprite("/cry.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void balloon() {
  int* dim = loadImageSprite("/balloon.png");
  drawImageSpriteDual((240-dim[0])/2,(240-dim[1])/2);
}

void heart() {
   digitalWrite (TFT_CS_0,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (80,80,50,RED);
   img.fillSmoothCircle (160,80,50,RED);
   for(int i=0;i<100;i++) img.drawLine(31+i,100,120,200,RED);
   for(int i=0;i<100;i++) img.drawLine(209-i,100,120,200,RED);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   digitalWrite (TFT_CS_1,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

void eye() {
   digitalWrite (TFT_CS_0,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,140,50,YELLOW);
   img.fillSmoothCircle (120,140,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   digitalWrite (TFT_CS_1,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,140,50,YELLOW);
   img.fillSmoothCircle (120,140,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

void righteye() {
   digitalWrite (TFT_CS_0,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (210,120,50,YELLOW);
   img.fillSmoothCircle (210,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   digitalWrite (TFT_CS_1,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

void lefteye() {
   digitalWrite (TFT_CS_0,LOW);
   img.fillSprite (BLACK);
   img.fillSmoothCircle (30,120,50,CYAN);
   img.fillSmoothCircle (30,120,20,BLACK);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   digitalWrite (TFT_CS_1,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

void sheepisheye() {
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,150,50,YELLOW);
   img.fillSmoothCircle (120,150,20,BLACK);
   
   for(int i=0;i<20;i++) img.drawLine(120,0+i,200,80+i,YELLOW);
   
   digitalWrite (TFT_CS_0,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,150,50,YELLOW);
   img.fillSmoothCircle (120,150,20,BLACK);
   
   for(int i=0;i<20;i++) img.drawLine(40,80+i,120,0+i,YELLOW);
 
   digitalWrite (TFT_CS_1,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

void angryeye() {
   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,200,50,RED);
   img.fillSmoothCircle (120,200,20,BLACK);
   
   for(int i=0;i<40;i++) img.drawLine(40,200+i,200,0+i,RED);
  
   digitalWrite (TFT_CS_0,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_0,HIGH);  

   img.fillSprite (BLACK);
   img.fillSmoothCircle (120,200,50,RED);
   img.fillSmoothCircle (120,200,20,BLACK);
   
   for(int i=0;i<40;i++) img.drawLine(200,200+i,40,0+i,RED);

   digitalWrite (TFT_CS_1,LOW);
   img.pushSprite(0, 10);
   digitalWrite (TFT_CS_1,HIGH);  
}

//Display libraries
#include "User_Setup.h" 
#include <SPI.h>
#include <TFT_eSPI.h> 

//PNG decoder library
#include <PNGdec.h>
#include "FS.h"
#include "SD.h"


//colours
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

#define MAX_IMAGE_WIDTH  240
#define MAX_IMAGE_HEIGHT 240

//TFT cont
#define TFT_CS_0 27
#define TFT_CS_1 33

void setupDisplay();
TFT_eSprite *getSprite();
TFT_eSPI *getTFT();

int pngDraw(PNGDRAW *pDraw);
void * pngOpen(const char *filename, int32_t *size);
void pngClose(void *handle);
int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length);
int32_t pngSeek(PNGFILE *page, int32_t position);
int* loadImageSprite(const char *name);
void drawImageSprite(int cs,int xpos, int ypos);
void drawImageSpriteDual(int xpos, int ypos);
void drawImageSpriteHorizontalMirror(int cs,int xpos, int ypos);

int pngDrawFLASH(PNGDRAW *pDraw);
int showImage(int disp,int img);

void heartpng();
void firework();
void snake();
void karen();
void arrow();
void cry();
void balloon();

void heart();
void eye();
void righteye();
void lefteye();
void sheepisheye();
void angryeye();

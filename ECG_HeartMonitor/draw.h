/*
 * ECG Display Functions Header
 * 
 * This file contains function declarations for drawing ECG waveforms,
 * grids, and medical display elements on the dual TFT displays.
 */

#ifndef DRAW_H
#define DRAW_H

// Display libraries
#include "User_Setup.h" 
#include <SPI.h>
#include <TFT_eSPI.h> 

// Colors for ECG display
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
#define DARK_GREEN  0x03E0
#define LIGHT_GREY  0x8410
#define DARK_RED    0x8000
#define DARK_BLUE   0x0010

// ECG Display constants
#define ECG_GRID_SIZE 20        // Grid spacing in pixels
#define ECG_TRACE_COLOR GREEN   // Color for ECG trace
#define ECG_GRID_COLOR GREY     // Color for grid lines
#define ECG_TEXT_COLOR WHITE    // Color for text
#define ECG_BG_COLOR BLACK      // Background color

// Display dimensions
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 280
#define ECG_AREA_HEIGHT 200     // Height of ECG plotting area
#define INFO_AREA_HEIGHT 80     // Height of information area

// Memory-optimized sprite dimensions
#define ECG_SPRITE_WIDTH 240
#define ECG_SPRITE_HEIGHT 200   // Reduced from 230 to fit memory constraints

// TFT control pins
#define TFT_CS_0 27
#define TFT_CS_1 33

// Function declarations
void setupDisplay();
TFT_eSprite *getECGSprite();
TFT_eSPI *getTFT();

// ECG drawing functions (using sprite)
void drawECGGrid();
void drawECGWaveform();
void clearECGTrace();
void drawECGPoint(int x, int y, uint16_t color);
void drawECGLine(int x1, int y1, int x2, int y2, uint16_t color);

// Information display functions (direct to display)
void drawECGLabels();
void drawHeartRate(int heartRate);
void drawLeadOffWarning();
void updateStatisticsDisplay();
void drawTimeStamp();

// Utility functions
void drawText(int x, int y, const char* text, uint16_t color, uint8_t size);
void drawRectangle(int x, int y, int width, int height, uint16_t color);
void fillRectangle(int x, int y, int width, int height, uint16_t color);
void drawHorizontalLine(int y, uint16_t color);
void drawVerticalLine(int x, uint16_t color);

// Dual display functions
void updateDisplay0();
void updateDisplay1();
void updateBothDisplays();
void updateInfoArea();

#endif // DRAW_H

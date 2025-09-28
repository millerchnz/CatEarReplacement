/*
 * ECG Display Functions Implementation
 * 
 * This file contains the implementation of functions for drawing ECG waveforms,
 * grids, and medical display elements on the dual TFT displays.
 */

#include "draw.h"

// Global TFT objects - Memory optimized
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite ecgSprite = TFT_eSprite(&tft);  // Only one sprite for ECG waveform

// ECG data buffer for drawing
extern float ecgBuffer[240];
extern int bufferIndex;
extern int heartRate;
extern float minValue, maxValue, avgValue;

void setupDisplay() {
  // Initialize chip select pins
  pinMode(TFT_CS_0, OUTPUT);
  pinMode(TFT_CS_1, OUTPUT);
  
  // Set both CS pins low for initialization
  digitalWrite(TFT_CS_0, LOW);
  digitalWrite(TFT_CS_1, LOW);
  
  // Initialize TFT
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(ECG_BG_COLOR);
  
  // Set CS pins high (inactive)
  digitalWrite(TFT_CS_0, HIGH);
  digitalWrite(TFT_CS_1, HIGH);
  
  // Create only one sprite for ECG waveform (memory optimized)
  ecgSprite.createSprite(ECG_SPRITE_WIDTH, ECG_SPRITE_HEIGHT);
  ecgSprite.setRotation(0);
  ecgSprite.fillSprite(ECG_BG_COLOR);
}

TFT_eSprite *getECGSprite() {
  return &ecgSprite;
}

TFT_eSPI *getTFT() {
  return &tft;
}

void drawECGGrid() {
  ecgSprite.fillSprite(ECG_BG_COLOR);
  
  // Draw horizontal grid lines
  for(int y = 0; y < ECG_SPRITE_HEIGHT; y += ECG_GRID_SIZE) {
    ecgSprite.drawFastHLine(0, y, ECG_SPRITE_WIDTH, ECG_GRID_COLOR);
  }
  
  // Draw vertical grid lines
  for(int x = 0; x < ECG_SPRITE_WIDTH; x += ECG_GRID_SIZE) {
    ecgSprite.drawFastVLine(x, 0, ECG_SPRITE_HEIGHT, ECG_GRID_COLOR);
  }
  
  // Update ECG sprite to display 0 only
  updateDisplay0();
}

void drawECGWaveform() {
  // Don't clear the grid, just draw the waveform
  int prevX = 0;
  int prevY = ECG_SPRITE_HEIGHT/2;
  
  for(int i = 0; i < ECG_SPRITE_WIDTH; i++) {
    int bufferIdx = (bufferIndex - ECG_SPRITE_WIDTH + i + 240) % 240;
    int x = i;

    // Double the amplitude for display (multiply by 2)
    float amplitudeMultiplier = 2.0;
    float adjustedValue = (ecgBuffer[bufferIdx] - ECG_BASELINE) * amplitudeMultiplier + ECG_BASELINE;
    int y = ECG_SPRITE_HEIGHT - (int)(adjustedValue * ECG_SPRITE_HEIGHT / 240);    
    
    // Clamp y to sprite bounds
    if(y < 0) y = 0;
    if(y >= ECG_SPRITE_HEIGHT) y = ECG_SPRITE_HEIGHT - 1;
    
    if(i > 0) {
      // Draw line from previous point to current point
      ecgSprite.drawLine(prevX, prevY, x, y, ECG_TRACE_COLOR);
    }
    
    prevX = x;
    prevY = y;
  }
  
  // Update ECG sprite to display 0 only
  updateDisplay0();
}

void clearECGTrace() {
  // Redraw the grid to clear the trace
  drawECGGrid();
}

void drawECGPoint(int x, int y, uint16_t color) {
  ecgSprite.drawPixel(x, y, color);
}

void drawECGLine(int x1, int y1, int x2, int y2, uint16_t color) {
  ecgSprite.drawLine(x1, y1, x2, y2, color);
}

void drawLeadOffWarning() {
  // Draw lead-off warning directly to display 1 only (circular display)
  digitalWrite(TFT_CS_1, LOW);
  tft.fillScreen(DARK_RED);  // Fill entire circular display with red
  tft.setTextColor(WHITE, DARK_RED);
  tft.setTextSize(2);
  tft.setCursor(60, 100);  // Centered horizontally and vertically
  tft.print("LEAD OFF!");
  
  tft.setTextSize(1);
  tft.setCursor(50, 130);  // Centered horizontally
  tft.print("Check electrode");
  tft.setCursor(60, 145);  // Centered horizontally
  tft.print("connections");
  digitalWrite(TFT_CS_1, HIGH);
}

void updateStatisticsDisplay() {
  // Statistics are now updated as part of updateInfoArea()
  // Limit info display updates to once every 1000-2000ms
  static unsigned long lastInfoUpdate = 0;
  unsigned long currentTime = millis();
  
  if(currentTime - lastInfoUpdate >= 1500) {  // Update every 1.5 seconds
    lastInfoUpdate = currentTime;
    updateInfoArea();
  }
}

void drawTimeStamp() {
  static unsigned long lastUpdate = 0;
  if(millis() - lastUpdate < 1000) return; // Update every second
  lastUpdate = millis();
  
  // Draw timestamp directly to display 1 only (circular display)
  digitalWrite(TFT_CS_1, LOW);
  tft.fillRect(60, 200, 80, 12, ECG_BG_COLOR);  // Clear area around timer
  tft.setTextColor(ECG_TEXT_COLOR, ECG_BG_COLOR);
  tft.setTextSize(1);
  tft.setCursor(60, 200);  // Centered horizontally, at bottom
  
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  
  if(minutes < 10) tft.print("0");
  tft.print(minutes);
  tft.print(":");
  if(seconds < 10) tft.print("0");
  tft.print(seconds);
  digitalWrite(TFT_CS_1, HIGH);
}


void drawText(int x, int y, const char* text, uint16_t color, uint8_t size) {
  // Draw text directly to display 1 only (info display)
  digitalWrite(TFT_CS_1, LOW);
  tft.setTextColor(color, ECG_BG_COLOR);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
  digitalWrite(TFT_CS_1, HIGH);
}

void drawRectangle(int x, int y, int width, int height, uint16_t color) {
  // Draw rectangle directly to display 1 only (info display)
  digitalWrite(TFT_CS_1, LOW);
  tft.drawRect(x, y, width, height, color);
  digitalWrite(TFT_CS_1, HIGH);
}

void fillRectangle(int x, int y, int width, int height, uint16_t color) {
  // Fill rectangle directly to display 1 only (info display)
  digitalWrite(TFT_CS_1, LOW);
  tft.fillRect(x, y, width, height, color);
  digitalWrite(TFT_CS_1, HIGH);
}

void drawHorizontalLine(int y, uint16_t color) {
  // Draw horizontal line directly to display 1 only (circular display)
  digitalWrite(TFT_CS_1, LOW);
  tft.drawFastHLine(0, y, DISPLAY_WIDTH, color);
  digitalWrite(TFT_CS_1, HIGH);
}

void drawVerticalLine(int x, uint16_t color) {
  // Draw vertical line directly to display 1 only (circular display)
  digitalWrite(TFT_CS_1, LOW);
  tft.drawFastVLine(x, 0, DISPLAY_HEIGHT, color);
  digitalWrite(TFT_CS_1, HIGH);
}

void updateDisplay0() {
  digitalWrite(TFT_CS_0, LOW);
  ecgSprite.pushSprite(0, 0);
  digitalWrite(TFT_CS_0, HIGH);
}

void updateDisplay1() {
  digitalWrite(TFT_CS_1, LOW);
  ecgSprite.pushSprite(0, 0);
  digitalWrite(TFT_CS_1, HIGH);
}

void updateBothDisplays() {
  // This function is no longer used since we separate ECG and info displays
  // ECG updates go to display 0, info updates go to display 1
}

void updateInfoArea() {
  // Draw complete info area with labels and current values (circular display)
  digitalWrite(TFT_CS_1, LOW);
  tft.fillScreen(ECG_BG_COLOR);  // Clear entire circular display
  
  // Draw heart rate value
  tft.setTextColor(ECG_TRACE_COLOR, ECG_BG_COLOR);
  tft.setTextSize(3);
  tft.setCursor(80, 60);  // Next to HR label
  if(heartRate > 0) {
    tft.print(heartRate);
    tft.print(" BPM");
  } else {
    tft.print(avgValue, 1);
    tft.print(" BPM");
  }
  
  // Draw statistics labels and values - arranged in center area
  tft.setTextColor(ECG_TEXT_COLOR, ECG_BG_COLOR);
  tft.setTextSize(2);
  
  // Min label and value
  tft.setCursor(60, 140);  // Left side of center
  tft.print("Min:");
  tft.print(minValue, 1);
  
  // Max label and value
  tft.setCursor(60, 120);  // Right side of center
  tft.print("Max:");
  tft.print(maxValue, 1);
  
  // Average label and value
  tft.setCursor(60, 160);  // Center bottom
  tft.print("Avg:");
  tft.print(avgValue, 1);
  
  digitalWrite(TFT_CS_1, HIGH);
}

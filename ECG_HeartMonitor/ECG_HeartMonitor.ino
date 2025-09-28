/*
 * ECG Heart Monitor with Dual Display
 * 
 * This project uses an AD8232 heart rate monitor to capture ECG signals
 * and displays them in real-time on two TFT displays like a medical ECG monitor.
 * 
 * Hardware:
 * - ESP32-WROOM32 microcontroller
 * - Two 240x280 ST7789 TFT displays
 * - AD8232 Heart Rate Monitor Module
 * - SD card for data logging (optional)
 * 
 * Pin Layout:
 * 
 * AD8232 Heart Monitor     ESP32
 *     GND ---------- GND
 *     VCC ---------- 3.3V
 *     OUTPUT ------- A0 (GPIO 36)
 *     LO+ ---------- GPIO 34
 *     LO- ---------- GPIO 35
 *     SDN ---------- GPIO 5
 * 
 * TFT Displays (shared SPI):
 *     GND ---------- GND
 *     VCC ---------- 3.3V
 *     SCL ---------- GPIO 18
 *     SDA ---------- GPIO 23
 *     RES ---------- GPIO 4
 *     DC  ---------- GPIO 2
 *     BLK ---------- 3.3V
 *     CS_0 --------- GPIO 27 (Display 0)
 *     CS_1 --------- GPIO 33 (Display 1)
 * 
 * SD Card (optional):
 *     GND ---------- GND
 *     VCC ---------- 3.3V
 *     MISO --------- GPIO 22
 *     MOSI --------- GPIO 17
 *     SCLK --------- GPIO 21
 *     CS ----------- GPIO 16
 * 
 * Author: Generated for CatEarReplacement Project
 * Date: 2024
 */

#include "draw.h"
#include <esp_heap_caps.h>

// AD8232 Heart Monitor pins
#define AD8232_OUTPUT_PIN A0    // GPIO 36 - Analog output from AD8232
#define AD8232_LO_PLUS    34    //14    // Lead-off detection positive
#define AD8232_LO_MINUS   35    //12    // Lead-off detection negative  
#define AD8232_SDN        5    // Shutdown pin

// ECG parameters
#define ECG_BUFFER_SIZE 240     // Buffer size for ECG data (matches display width)
#define ECG_SAMPLE_RATE 250     // Sample rate in Hz
#define ECG_GAIN 100            // Display gain multiplier
#define ECG_BASELINE 100        // Baseline position on display (adjusted for smaller sprite)

// Global variables
float ecgBuffer[ECG_BUFFER_SIZE];
int bufferIndex = 0;
unsigned long lastSampleTime = 0;
unsigned long sampleInterval = 1000000 / ECG_SAMPLE_RATE; // microseconds
bool isRecording = false;
int heartRate = 0;
unsigned long lastHeartBeat = 0;
int heartRateBuffer[10];
int heartRateIndex = 0;

// Statistics
float minValue = 0;
float maxValue = 0;
float avgValue = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize AD8232 pins
  pinMode(AD8232_LO_PLUS, INPUT);
  pinMode(AD8232_LO_MINUS, INPUT);
  pinMode(AD8232_SDN, OUTPUT);
  
  // Enable AD8232 (SDN = LOW)
  digitalWrite(AD8232_SDN, HIGH);
  
  // Initialize ECG buffer
  for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
    ecgBuffer[i] = ECG_BASELINE;
  }
  
  // Initialize heart rate buffer
  for(int i = 0; i < 10; i++) {
    heartRateBuffer[i] = 0;
  }
  
  // Initialize displays
  setupDisplay();
  
  // Draw initial ECG grid
  drawECGGrid();

  Serial.println("ECG Heart Monitor Initialized");
  Serial.println("Place electrodes on chest and wait for signal...");
  
  isRecording = true;
}

void loop() {
  // Check for lead-off detection
  if(digitalRead(AD8232_LO_PLUS) == HIGH || digitalRead(AD8232_LO_MINUS) == HIGH) {
    // Lead-off detected - show warning
    showLeadOffWarning();
    delay(100);
    return;
  }
  
  // Sample ECG data at specified rate
  unsigned long currentTime = micros();
  if(currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;
    
    // Read analog value from AD8232
    int rawValue = analogRead(AD8232_OUTPUT_PIN);
    
    // Convert to voltage (0-3.3V range, 12-bit ADC)
    float voltage = (rawValue * 3.3) / 4095.0;
    
    // Apply gain and offset for display
    float ecgValue = (voltage - 1.65) * ECG_GAIN + ECG_BASELINE;
        
    // Add to buffer
    ecgBuffer[bufferIndex] = ecgValue;
    bufferIndex = (bufferIndex + 1) % ECG_BUFFER_SIZE;
    
    // Update statistics
    updateStatistics(ecgValue);
    
    // Detect heart beats (simple peak detection)
    detectHeartBeat(ecgValue);
    
    // Update display every few samples (halved refresh rate)
    if(bufferIndex % 8 == 0) {
      updateECGDisplay();
    }
  }
  
  // Small delay to prevent overwhelming the system
  delay(1);
}

void updateStatistics(float value) {
  static float sum = 0;
  static int count = 0;
  
  if(count < ECG_BUFFER_SIZE) {
    sum += value;
    count++;
    avgValue = sum / count;
  } else {
    sum = sum - ecgBuffer[(bufferIndex + 1) % ECG_BUFFER_SIZE] + value;
    avgValue = sum / ECG_BUFFER_SIZE;
  }
  
  if(value < minValue && value > 0) minValue = value;
  if(value > maxValue) maxValue = value;
}

void detectHeartBeat(float value) {
  static float lastValue = ECG_BASELINE;
  static bool rising = false;
  static float threshold = 20.0; // Minimum amplitude for heartbeat detection
  
  // Simple peak detection algorithm
  if(value > lastValue && !rising) {
    rising = true;
  } else if(value < lastValue && rising) {
    rising = false;
    
    // Check if this is a significant peak
    if(value > ECG_BASELINE + threshold) {
      unsigned long currentTime = millis();
      
      if(lastHeartBeat > 0) {
        // Calculate heart rate (beats per minute)
        unsigned long interval = currentTime - lastHeartBeat;
        if(interval > 300 && interval < 2000) { // Valid heart rate range
          int currentHR = 60000 / interval;
          
          // Add to heart rate buffer
          heartRateBuffer[heartRateIndex] = currentHR;
          heartRateIndex = (heartRateIndex + 1) % 10;
          
          // Calculate average heart rate
          int sum = 0;
          int validReadings = 0;
          for(int i = 0; i < 10; i++) {
            if(heartRateBuffer[i] > 0) {
              sum += heartRateBuffer[i];
              validReadings++;
            }
          }
          
          if(validReadings > 0) {
            heartRate = sum / validReadings;
          }
        }
      }
      
      lastHeartBeat = currentTime;
    }
  }
  
  lastValue = value;
}

void showLeadOffWarning() {
  // Display lead-off warning on both screens
  drawLeadOffWarning();
}

void updateECGDisplay() {
  // Clear the ECG trace area
  clearECGTrace();
  
  // Draw the ECG waveform
  drawECGWaveform();
  
  // Update statistics display
  updateStatisticsDisplay();
}

# ECG Heart Monitor with Dual Display

This project creates a real-time ECG (Electrocardiogram) heart monitor using an AD8232 heart rate sensor and dual TFT displays, similar to medical ECG equipment.

Prompts:
 - describe this project
 - Are you able to draw a circuit diagram based on pins used?
 - Add another folder for a new use of the dual displays and write code to pull signals from an AD8232 heart monitor and plot readings on the dual displays much like an ECG
 - This code uses two large sprites that use more RAM than the ESP32 I'm using has. It only has enough memory for a 240x230 pixel sprite. Please change the code to allow for the tighter memory constraints. Maybe you could write info directly to the info display and use the sprite for the ECG wave form?
 - I have changed AD8232_LO_PLUS to pin 34, AD8232_LO_MINUS to pin 35 and AD8232_SDN to pin 5 as the suggested pins were in use. Please check my changes to see if this will break any functions
 - what does drawBatteryLevel function do? Is it necessary?
 - yes, please remove the drawBatteryLevel function
 - draw.cpp write ecg wave form and info statistics to both dsiplays. refactor this code to put the ECG wave form on display 0 and the info on display 1 only. The number of times  TFT_CS_0 and TFT_CS_1 are changed  should be limited to as few times as possible so that updates are grouped together
 - UpdateInfoArea uses ECG_SPRITE_HEIGHT as it assumes the ECG display i above it. his doesn't make sense - its should start drawing from the top of display 1. Refactor the function to start printing info from the top of the screen. Also, note that the display is circular, so shft the text further to the middle at the top and bottom of the display
 - Halve the refresh rate of the ecg wave form
 - Display 1 is updating too often. Roughly once every 1000ms to 2000ms is fine
 - SDN is set incorrectly as LOW to initialise - this needs to be set HIGH to enable the device


## Features

- **Real-time ECG waveform display** on dual 240x280 TFT displays
- **Heart rate detection and display** with BPM calculation
- **Lead-off detection** with visual warnings
- **Statistics display** showing min, max, and average values
- **Medical-style grid display** for easy waveform interpretation
- **Dual display support** for simultaneous monitoring

## Hardware Requirements

### Microcontroller
- ESP32-WROOM32 development board

### Displays
- Two 240x280 pixel ST7789 TFT displays (1.69 inch)
- GC9A01 driver compatible

### Heart Monitor
- AD8232 Heart Rate Monitor Module
- ECG electrodes (3-lead system)

### Optional
- SD card module for data logging
- Breadboard and jumper wires

## Pin Connections

### AD8232 Heart Monitor to ESP32
```
AD8232 Module    ESP32 Pin    Function
GND              GND          Ground
VCC              3.3V         Power
OUTPUT           A0 (GPIO 36) Analog ECG signal
LO+              GPIO 34      Lead-off detection positive
LO-              GPIO 35      Lead-off detection negative
SDN              GPIO 5       Shutdown control
```

### TFT Displays to ESP32 (Shared SPI)
```
Display Pins     ESP32 Pin    Function
GND              GND          Ground
VCC              3.3V         Power
SCL              GPIO 18      SPI Clock
SDA              GPIO 23      SPI MOSI
RES              GPIO 4       Reset
DC               GPIO 2       Data/Command
BLK              3.3V         Backlight
CS_0             GPIO 27      Display 0 Chip Select
CS_1             GPIO 33      Display 1 Chip Select
```

### SD Card (Optional)
```
SD Card Pins     ESP32 Pin    Function
GND              GND          Ground
VCC              3.3V         Power
MISO             GPIO 22      SPI MISO
MOSI             GPIO 17      SPI MOSI
SCLK             GPIO 21      SPI Clock
CS               GPIO 16      Chip Select
```

## Software Setup

1. **Install Required Libraries:**
   - TFT_eSPI (for display control)
   - Arduino IDE with ESP32 board support

2. **Configure TFT_eSPI:**
   - Edit `User_Setup.h` to match your display driver
   - Ensure GC9A01_DRIVER is selected
   - Verify pin assignments match your hardware

3. **Upload Code:**
   - Open `ECG_HeartMonitor.ino` in Arduino IDE
   - Select ESP32 board and correct COM port
   - Upload the code

## Usage

1. **Hardware Setup:**
   - Connect all components according to pin diagram
   - Attach ECG electrodes to the AD8232 module
   - Power on the ESP32

2. **Electrode Placement:**
   - **Right Arm (RA):** Place on right wrist or upper arm
   - **Left Arm (LA):** Place on left wrist or upper arm  
   - **Right Leg (RL):** Place on right ankle or leg (ground reference)

3. **Operation:**
   - The displays will show a grid pattern initially
   - Once electrodes are properly connected, ECG waveform will appear
   - Heart rate will be calculated and displayed in real-time
   - Lead-off warnings will appear if electrodes become disconnected

## Display Information

### Display 0 - ECG Waveform (Full 240x280 pixels)
- Green grid lines for easy measurement
- Real-time ECG trace in green
- Baseline reference line in white
- 240-pixel wide display showing recent ECG data
- Dedicated to ECG waveform visualization only

### Display 1 - Information Panel (Circular 240x280 pixels)
- **Title:** "ECG Monitor" (centered at top)
- **Heart Rate:** Current BPM with "BPM" label (centered)
- **Statistics:** Min, Max, and Average values (arranged in center)
- **Lead Status:** Connection status indicator (centered at bottom)
- **Timer:** Recording time display (centered at bottom)
- Dedicated to monitoring information and status
- Optimized for circular display with centered text positioning

## Technical Specifications

- **Sample Rate:** 250 Hz (4ms intervals)
- **Display Update:** 60 FPS
- **Heart Rate Range:** 30-200 BPM
- **ECG Gain:** 100x amplification
- **Buffer Size:** 240 samples (1 second at 250Hz)
- **Lead-off Detection:** Automatic with visual warning
- **Memory Optimized:** Uses single 240x200 sprite for ECG waveform, info written directly to display
- **Dual Display Separation:** ECG waveform on display 0, monitoring info on display 1

## Troubleshooting

### No ECG Signal
- Check electrode connections
- Ensure electrodes have good skin contact
- Verify AD8232 power and connections
- Check serial monitor for error messages

### Display Issues
- Verify TFT_eSPI library installation
- Check User_Setup.h configuration
- Ensure correct pin connections
- Try different SPI frequency settings

### Heart Rate Not Detected
- Ensure ECG signal is strong enough
- Check electrode placement
- Verify signal amplitude in serial output
- Adjust detection threshold if needed

## Safety Notes

⚠️ **Important:** This is an educational project and should not be used for medical diagnosis or treatment. Always consult healthcare professionals for medical concerns.

- Use only for educational and hobby purposes
- Do not rely on readings for medical decisions
- Ensure proper electrical isolation
- Follow all safety guidelines for electronic projects

## Future Enhancements

- Data logging to SD card
- Bluetooth connectivity for remote monitoring
- Heart rate variability analysis
- Multiple lead configurations
- Alarm system for abnormal rhythms
- Web interface for remote access

## License

This project is provided as-is for educational purposes. Use at your own risk.

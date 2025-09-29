# CatEarReplacement - ESP32 Dual Display Project

A versatile Arduino/ESP32-based project featuring dual TFT displays (240x280 pixels) with multiple applications including medical ECG monitoring, image display, and interactive animations. This project appears to be designed as a replacement for "cat ears" - likely a wearable device or display system.

## 🚀 Project Overview

This repository contains several ESP32 applications that demonstrate the capabilities of dual TFT displays with the GC9A01 driver. The project includes everything from basic multi-threading examples to professional medical-grade ECG monitoring systems.

## 📁 Project Structure

### Applications

| Folder | Description | Key Features |
|--------|-------------|--------------|
| `BasicMultiThreading/` | FreeRTOS multi-threading demo | LED blinking + analog reading |
| `DualDisplays/` | Basic dual display image viewer | PNG images from SD card |
| `BLESerialDisp/` | Bluetooth-controlled display | Remote image selection via BT |
| `FlashBLEDisp/` | Flash-based image display | Pre-compiled image headers |
| `ECG_HeartMonitor/` | Medical ECG monitoring system | Real-time heart rate monitoring |
| `ESP32_two_240x280_ST7789/` | Basic dual display demo | Eye animations and patterns |

### Libraries

| Library | Purpose | Version |
|---------|---------|---------|
| `TFT_eSPI/` | TFT display control | Latest |
| `PNGdec/` | PNG image decoding | Latest |

## 🔧 Hardware Requirements

### Core Components
- **ESP32-WROOM32** development board
- **Two 240x280 pixel TFT displays** with GC9A01 drivers (circular)
- **SD card module** (optional, for image storage)
- **AD8232 Heart Rate Monitor Module** (for ECG application)
- **ECG electrodes** (3-lead system for medical monitoring)

### Optional Components
- Breadboard and jumper wires
- Power supply (3.3V/5V)
- Bluetooth-enabled device for remote control

## 🔌 Pin Configuration

### ESP32 to TFT Displays (Shared SPI)
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

### ESP32 to SD Card
```
SD Card Pins     ESP32 Pin    Function
GND              GND          Ground
VCC              3.3V         Power
MISO             GPIO 22      SPI MISO
MOSI             GPIO 17      SPI MOSI
SCLK             GPIO 21      SPI Clock
CS               GPIO 16      Chip Select
```

### ESP32 to AD8232 Heart Monitor (ECG Application)
```
AD8232 Module    ESP32 Pin    Function
GND              GND          Ground
VCC              3.3V         Power
OUTPUT           A0 (GPIO 36) Analog ECG signal
LO+              GPIO 34      Lead-off detection positive
LO-              GPIO 35      Lead-off detection negative
SDN              GPIO 5       Shutdown control
```

## 📊 Circuit Diagrams

### Complete System Wiring Diagram
```
                    ┌─────────────────────────────────────────────────┐
                    │                ESP32-WROOM32                    │
                    │                                                 │
                    │  GPIO 18 ──── SCLK ────┐                       │
                    │  GPIO 23 ──── MOSI ────┤                       │
                    │  GPIO 22 ──── MISO ────┤                       │
                    │  GPIO 4  ──── RES  ────┤                       │
                    │  GPIO 2  ──── DC   ────┤                       │
                    │  GPIO 27 ──── CS0  ────┤                       │
                    │  GPIO 33 ──── CS1  ────┤                       │
                    │  GPIO 16 ──── SDCS ────┤                       │
                    │  GPIO 36 ──── A0   ────┤                       │
                    │  GPIO 34 ──── LO+  ────┤                       │
                    │  GPIO 35 ──── LO-  ────┤                       │
                    │  GPIO 5  ──── SDN  ────┤                       │
                    │  3.3V   ──── VCC  ────┤                       │
                    │  GND    ──── GND  ────┤                       │
                    └─────────────────────────┼─────────────────────┘
                                              │
                    ┌─────────────────────────┼─────────────────────┐
                    │                         │                     │
        ┌───────────▼───────────┐    ┌────────▼────────┐    ┌──────▼──────┐
        │    TFT Display 0      │    │  TFT Display 1   │    │  SD Card    │
        │   (240x280 GC9A01)    │    │ (240x280 GC9A01) │    │   Module    │
        │                       │    │                  │    │             │
        │  VCC ──── 3.3V        │    │ VCC ──── 3.3V    │    │ VCC ──── 3.3V│
        │  GND ──── GND         │    │ GND ──── GND     │    │ GND ──── GND │
        │  SCL ──── GPIO 18     │    │ SCL ──── GPIO 18 │    │ SCLK ─── GPIO 21│
        │  SDA ──── GPIO 23     │    │ SDA ──── GPIO 23 │    │ MOSI ─── GPIO 17│
        │  RES ──── GPIO 4      │    │ RES ──── GPIO 4  │    │ MISO ─── GPIO 22│
        │  DC  ──── GPIO 2      │    │ DC  ──── GPIO 2  │    │ CS   ─── GPIO 16│
        │  CS  ──── GPIO 27     │    │ CS  ──── GPIO 33 │    │             │
        │  BLK ──── 3.3V        │    │ BLK ──── 3.3V    │    │             │
        └───────────────────────┘    └──────────────────┘    └──────────────┘
```

### ECG Heart Monitor System
```
                    ┌─────────────────────────────────────────────────┐
                    │                ESP32-WROOM32                    │
                    │                                                 │
                    │  GPIO 36 ──── A0   ────┐                       │
                    │  GPIO 34 ──── LO+  ────┤                       │
                    │  GPIO 35 ──── LO-  ────┤                       │
                    │  GPIO 5  ──── SDN  ────┤                       │
                    │  3.3V   ──── VCC  ────┤                       │
                    │  GND    ──── GND  ────┤                       │
                    └─────────────────────────┼─────────────────────┘
                                              │
                    ┌─────────────────────────▼─────────────────────┐
                    │              AD8232 Heart Monitor             │
                    │                                               │
                    │  VCC ──── 3.3V                               │
                    │  GND ──── GND                                │
                    │  OUTPUT ──── A0 (GPIO 36)                    │
                    │  LO+ ──── GPIO 34                            │
                    │  LO- ──── GPIO 35                            │
                    │  SDN ──── GPIO 5                             │
                    └─────────────────────────┬─────────────────────┘
                                              │
                    ┌─────────────────────────▼─────────────────────┐
                    │            ECG Electrodes (3-Lead)            │
                    │                                               │
                    │  Right Arm (RA) ──── LO+                     │
                    │  Left Arm (LA)  ──── LO-                     │
                    │  Right Leg (RL) ──── GND (Reference)         │
                    └───────────────────────────────────────────────┘
```

### Power Distribution
```
                    ┌─────────────────────────────────────────────────┐
                    │                Power Supply                     │
                    │              (3.3V/5V Input)                   │
                    └─────────────────────────┬─────────────────────┘
                                              │
                    ┌─────────────────────────▼─────────────────────┐
                    │                ESP32-WROOM32                   │
                    │              (3.3V Regulator)                 │
                    └─────────────────────────┬─────────────────────┘
                                              │
                    ┌─────────────────────────▼─────────────────────┐
                    │           3.3V Distribution Bus               │
                    │                                               │
                    │  ┌─────────┐  ┌─────────┐  ┌─────────┐       │
                    │  │ Display │  │ Display │  │   SD    │       │
                    │  │    0    │  │    1    │  │  Card   │       │
                    │  └─────────┘  └─────────┘  └─────────┘       │
                    │                                               │
                    │  ┌─────────┐  ┌─────────┐                    │
                    │  │ AD8232  │  │  Other  │                    │
                    │  │  ECG    │  │ Modules │                    │
                    │  └─────────┘  └─────────┘                    │
                    └───────────────────────────────────────────────┘
```

## 🚀 Getting Started

### Prerequisites
1. **Arduino IDE** with ESP32 board support
2. **Required Libraries:**
   - TFT_eSPI
   - PNGdec
   - BluetoothSerial (built-in)

### Installation
1. Clone this repository
2. Install required libraries via Arduino IDE Library Manager
3. Configure `User_Setup.h` files for your specific display configuration
4. Select the appropriate ESP32 board in Arduino IDE
5. Upload the desired application

### Configuration
Each application has its own `User_Setup.h` file. Key settings:
- Display driver: `GC9A01_DRIVER`
- SPI frequency: `27000000` (27MHz)
- Pin assignments as shown in pin configuration above

## 📱 Applications

### 1. Basic Multi-Threading
**File:** `BasicMultiThreading/BasicMultiThreading.ino`
- Demonstrates FreeRTOS multi-threading
- Blinks LED while reading analog input
- Educational example for ESP32 parallel processing
- Shows task creation and management

### 2. Dual Display Image Viewer
**Files:** `DualDisplays/`, `BLESerialDisp/`, `FlashBLEDisp/`
- Displays PNG images from SD card
- Bluetooth control for image selection
- Supports various animations and effects
- Pre-loaded images: hearts, fireworks, snakes, balloons
- Horizontal mirroring between displays
- Memory-efficient sprite rendering

### 3. ECG Heart Monitor
**File:** `ECG_HeartMonitor/ECG_HeartMonitor.ino`
- **Medical-grade ECG monitoring**
- Real-time waveform display
- Heart rate detection (30-200 BPM)
- Lead-off detection with warnings
- Statistics display (min, max, average)
- Memory-optimized for ESP32 constraints

**Features:**
- 250Hz sampling rate
- Professional medical grid display
- Dual display separation (ECG waveform + info)
- Real-time heart rate calculation
- Lead-off detection and warnings
- Circular display optimization
- Memory-constrained sprite management

### 4. Basic Dual Display Demo
**File:** `ESP32_two_240x280_ST7789/ESP32_two_240x280_ST7789.ino`
- Simple dual display demonstration
- Eye expressions and animations
- Geometric patterns and shapes
- Educational display programming
- Heart shapes and emotional expressions

## 🔧 Technical Specifications

### Display Specifications
- **Resolution:** 240x280 pixels each
- **Driver:** GC9A01
- **Interface:** SPI
- **Color Depth:** 16-bit RGB565
- **Update Rate:** 60 FPS

### Performance
- **ECG Sampling:** 250Hz
- **Display Updates:** 60 FPS
- **Memory:** Optimized for ESP32 constraints
- **Bluetooth:** 115200 baud

### Memory Optimization
- Single sprite for ECG waveform (240x200 pixels)
- Direct display writes for information
- PSRAM utilization where available

## 🎯 Use Cases

1. **Wearable Display System** - Head-mounted or wearable applications
2. **Medical Monitoring** - Professional ECG heart monitoring
3. **Interactive Display** - Bluetooth-controlled image and animation display
4. **Educational Platform** - ESP32 multi-threading and display programming
5. **Art/Entertainment** - Visual effects and animations

## ⚠️ Safety Notes

**Important:** The ECG Heart Monitor is for educational purposes only and should not be used for medical diagnosis or treatment. Always consult healthcare professionals for medical concerns.

- Use only for educational and hobby purposes
- Do not rely on readings for medical decisions
- Ensure proper electrical isolation
- Follow all safety guidelines for electronic projects

## 🔧 Troubleshooting

### Common Issues

**Display Not Working:**
- Check pin connections according to circuit diagrams
- Verify `User_Setup.h` configuration
- Ensure correct SPI frequency (27MHz)
- Check power supply (3.3V)

**ECG Signal Issues:**
- Verify electrode connections
- Check AD8232 power and connections
- Ensure good skin contact with electrodes
- Verify pin assignments (A0, LO+, LO-, SDN)

**Bluetooth Connection:**
- Ensure Bluetooth is enabled in ESP32 configuration
- Check device name: "ESP32DualDisplay"
- Verify baud rate: 115200

**Memory Issues:**
- Use PSRAM if available
- Reduce sprite sizes for memory-constrained applications
- Check heap usage in serial monitor

### Debugging Tips

1. **Serial Monitor:** Use 115200 baud for debugging
2. **Memory Usage:** Monitor heap usage with `heap_caps_get_free_size()`
3. **Display Test:** Use basic drawing functions to test displays
4. **Pin Testing:** Use multimeter to verify connections

## 🔮 Future Enhancements

- Data logging to SD card
- Enhanced Bluetooth connectivity
- Heart rate variability analysis
- Multiple lead configurations
- Alarm system for abnormal rhythms
- Web interface for remote access
- Machine learning for pattern recognition
- Wireless charging support
- Real-time data streaming

## 📄 License

This project is provided as-is for educational purposes. Use at your own risk.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## 📞 Support

For questions and support, please open an issue in this repository.

---

**Note:** This project demonstrates advanced ESP32 capabilities with dual displays and includes both educational examples and practical medical monitoring applications. The modular design allows for easy customization and extension.
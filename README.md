# BMO Embedded Display Project 🤖

A delightful Arduino project that brings BMO (from Adventure Time) to life on a 2.4" LCD display! This project creates an animated BMO face that blinks, smiles, and displays various expressions.

## ✨ Features

- **Authentic BMO Character Design**: Teal background with signature rectangular frame
- **Animated Expressions**: Happy, surprised, sleepy, excited, and confused faces
- **Realistic Blinking**: Random blink intervals with smooth eye animations
- **High-Quality Graphics**: 240×320 pixel color display with smooth curves
- **Modular Code Architecture**: Clean separation of display, graphics, and main logic
- **Hardware Abstraction**: Supports both ILI9341 and ST7789 display controllers
- **Performance Optimized**: Fast SPI communication with ESP32 hardware acceleration

## 🛠 Hardware Requirements

| Component | Purpose | Link |
|-----------|---------|------|
| **Arduino Nano ESP32** | Microcontroller brain | [Arduino Store](https://store-usa.arduino.cc/products/nano-esp32) |
| **Waveshare 2.4" LCD** | 240×320 color display | [Waveshare Store](https://www.waveshare.com/2.4inch-lcd-module.htm) |
| **Breadboard + Jumper Wires** | Connections | [SparkFun](https://www.sparkfun.com/products/12794) |
| **Soldering Kit** | For Arduino headers | Required for assembly |
| **USB-C Cable** | Power and programming | Standard cable |

## 🚀 Quick Start

### 1. Hardware Setup
```bash
# See detailed wiring guide in docs/wiring_diagram.md
Arduino D10 → LCD CS
Arduino D9  → LCD RESET  
Arduino D8  → LCD DC
Arduino D11 → LCD MOSI
Arduino D13 → LCD SCK
Arduino D7  → LCD LED (backlight)
Arduino 3.3V → LCD VCC
Arduino GND  → LCD GND
```

### 2. Software Setup
```bash
# Install Arduino IDE and ESP32 board support
# Install TFT_eSPI library
# Copy User_Setup.h to TFT_eSPI library folder
# Upload BMO sketch to Arduino
```

### 3. Run BMO!
- Power on the Arduino Nano ESP32
- Watch BMO come to life with animated expressions
- Enjoy the nostalgic Adventure Time vibes! 

## 📁 Project Structure

```
bmo-embedded/
├── src/
│   ├── bmo_display.ino     # Main Arduino sketch
│   ├── display.h           # Display driver interface
│   ├── display.cpp         # Hardware abstraction layer
│   ├── graphics.h          # BMO drawing functions
│   └── graphics.cpp        # Graphics implementation
├── config/
│   ├── User_Setup.h        # TFT_eSPI configuration
│   └── platformio.ini      # PlatformIO build config
├── docs/
│   ├── wiring_diagram.md   # Complete assembly guide
│   └── troubleshooting.md  # Common issues & solutions
├── examples/
│   └── simple_test.ino     # Basic display test
└── README.md              # This file
```

## 🎨 BMO Expressions

The project includes multiple BMO expressions:

- **😊 Happy**: Classic BMO smile with bright eyes
- **😲 Surprised**: Wide eyes with open mouth  
- **😴 Sleepy**: Half-closed eyes with small mouth
- **🤩 Excited**: Large eyes with big grin
- **🤔 Confused**: Normal eyes with wavy mouth

## 🔧 Customization

### Adding New Expressions
```cpp
// In graphics.cpp, add new expression function
void BMOGraphics::drawCustomMouth(int centerX, int centerY) {
  // Your custom mouth drawing code here
}

// In graphics.h, add to BMOExpression enum
enum BMOExpression {
  EXPRESSION_HAPPY = 0,
  EXPRESSION_CUSTOM,  // Add your expression
  // ... existing expressions
};
```

### Changing Colors
```cpp
// Modify color palette in graphics.h
#define BMO_TEAL     0x4E6D    // Original teal
#define BMO_PURPLE   0x8010    // Custom purple BMO
#define BMO_GOLDEN   0xFEA0    // Golden BMO variant
```

### Animation Timing
```cpp
// In main sketch, adjust timing variables
unsigned long blinkInterval = 3000;  // Change blink frequency
int animationSpeed = 150;             // Adjust animation speed
```

## 📊 Performance Specifications

- **Display Resolution**: 240×320 pixels (16-bit color)
- **Frame Rate**: ~30 FPS for animations
- **Memory Usage**: ~150KB RAM for full frame buffer
- **SPI Speed**: 27MHz (optimized for ESP32)
- **Power Consumption**: ~200mA @ 3.3V (backlight on)
- **Boot Time**: <2 seconds to first BMO face

## 🐛 Troubleshooting

### Common Issues

**Display stays black:**
- Check 3.3V power connection (NOT 5V!)
- Verify TFT_eSPI User_Setup.h pin configuration
- Test with simple color fill example

**Garbled display:**
- Check MOSI and SCK SPI connections
- Verify CS and DC control pins
- Try slower SPI frequency

**No blinking animation:**
- Check serial monitor for error messages
- Verify graphics library initialization
- Test with static face first

See `docs/troubleshooting.md` for detailed solutions.

## 🤝 Contributing

Contributions welcome! Areas for improvement:

- Additional BMO expressions and animations
- Sound effects integration
- Touch/button interaction
- Power optimization features
- Custom BMO case designs

## 📜 License

This project is open source and available under the MIT License.

## 🎬 Adventure Time Connection

> "BMO is a living prototype video game console system, portable electrical outlet, computer, music player, VCR, video player, video editor, roommate, camera, alarm clock, toaster, flashlight, strobe light, skateboarder, soccer player, tape player, chef, detective and friend who is owned by Finn and Jake."

This project captures BMO's cheerful personality in embedded form! 

## 📚 Technical References

- [Arduino Nano ESP32 Documentation](https://docs.arduino.cc/hardware/nano-esp32)
- [TFT_eSPI Library Guide](https://github.com/Bodmer/TFT_eSPI)
- [ESP32 SPI Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)
- [ILI9341 Display Controller Datasheet](https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf)

---

**Made with ❤️ for Adventure Time fans and embedded systems enthusiasts!**

*"Oh my glob, this is algebraic!"* - BMO (probably)
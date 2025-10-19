# BMO Display Project - Wiring Diagram & Assembly Guide

## Components Required

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Nano ESP32 | 1 | Main microcontroller |
| Waveshare 2.4" LCD Module | 1 | Display screen (240×320 pixels) |
| Solderless Breadboard (Half-size or Full) | 1 | Component connections |
| Male-to-Female Jumper Wires | 9 | LCD to Arduino connections |
| USB-C Cable | 1 | Power and programming |
| Soldering Kit | 1 | For Arduino header pins |

## Pin Connections

### Arduino Nano ESP32 to Waveshare 2.4" LCD

| LCD Module Pin | Arduino Nano ESP32 Pin | GPIO Number | Function | Wire Color (Suggested) |
|----------------|----------------------|-------------|----------|----------------------|
| **VCC** | **3.3V** | - | Power Supply (+3.3V) | **Red** |
| **GND** | **GND** | - | Ground (0V) | **Black** |
| **CS** | **D10** | GPIO10 | SPI Chip Select | **Purple** |
| **RESET** | **D9** | GPIO9 | Display Reset | **White** |
| **DC** | **D8** | GPIO8 | Data/Command Select | **Gray** |
| **SDI (MOSI)** | **D11** | GPIO11 | SPI Data Out | **Blue** |
| **SCK** | **D13** | GPIO13 | SPI Clock | **Green** |
| **LED** | **D7** | GPIO7 | Backlight Control | **Yellow** |
| **SDO (MISO)** | **D12** | GPIO12 | SPI Data In (Optional) | **Orange** |

## Visual Wiring Diagram

```
Arduino Nano ESP32                    Waveshare 2.4" LCD
┌─────────────────┐                  ┌─────────────────┐
│                 │                  │                 │
│  3.3V  ●────────┼──────(Red)───────┼────●  VCC      │
│                 │                  │                 │
│  GND   ●────────┼──────(Black)─────┼────●  GND      │
│                 │                  │                 │
│  D7    ●────────┼──────(Yellow)────┼────●  LED      │
│  D8    ●────────┼──────(Gray)──────┼────●  DC       │
│  D9    ●────────┼──────(White)─────┼────●  RESET    │
│  D10   ●────────┼──────(Purple)────┼────●  CS       │
│  D11   ●────────┼──────(Blue)──────┼────●  SDI      │
│  D12   ●────────┼──────(Orange)────┼────●  SDO      │
│  D13   ●────────┼──────(Green)─────┼────●  SCK      │
│                 │                  │                 │
│  USB-C ●────────┼──── To Computer  │                 │
└─────────────────┘                  └─────────────────┘
```

## Step-by-Step Assembly Instructions

### Step 1: Prepare the Arduino Nano ESP32

1. **Solder Header Pins** (REQUIRED)
   - The Arduino Nano ESP32 typically ships without header pins
   - Solder male header pins to both sides of the board
   - Ensure pins are straight and properly aligned
   - Allow to cool completely before handling

2. **Test the Arduino**
   - Connect via USB-C to computer
   - Upload a simple "Blink" sketch to verify functionality
   - Disconnect after successful test

### Step 2: Prepare the Breadboard

1. **Insert Arduino into Breadboard**
   - Place Arduino Nano ESP32 across the center divider
   - Ensure all pins make good contact with breadboard

2. **Power Rail Setup**
   - Connect Arduino's **3.3V** pin to the positive power rail (red)
   - Connect Arduino's **GND** pin to the negative power rail (black)

### Step 3: Connect the LCD Display

**CRITICAL: Double-check all connections before powering on!**

1. **Power Connections** (Most Critical)
   ```
   LCD VCC  → Arduino 3.3V (RED wire)
   LCD GND  → Arduino GND  (BLACK wire)
   ```
   
   ⚠️ **WARNING**: The Waveshare 2.4" LCD is designed for 3.3V operation. 
   Do NOT connect to 5V as this may damage the display!

2. **SPI Control Pins**
   ```
   LCD CS    → Arduino D10 (GPIO10) - PURPLE wire
   LCD RESET → Arduino D9  (GPIO9)  - WHITE wire  
   LCD DC    → Arduino D8  (GPIO8)  - GRAY wire
   ```

3. **SPI Data Pins**
   ```
   LCD SDI (MOSI) → Arduino D11 (GPIO11) - BLUE wire
   LCD SCK        → Arduino D13 (GPIO13) - GREEN wire
   LCD SDO (MISO) → Arduino D12 (GPIO12) - ORANGE wire (Optional)
   ```

4. **Backlight Control**
   ```
   LCD LED → Arduino D7 (GPIO7) - YELLOW wire
   ```

### Step 4: Verify Connections

Before powering on, verify each connection:

- [ ] VCC → 3.3V (NOT 5V!)
- [ ] GND → GND
- [ ] All 7-9 signal wires properly connected
- [ ] No short circuits between adjacent pins
- [ ] All connections secure in breadboard

### Step 5: Initial Testing

1. **Connect USB-C Cable**
   - Arduino should power on (LED indicator should light)
   - Display backlight may briefly flash

2. **Upload Test Sketch**
   ```cpp
   // Simple test to verify display connection
   #include <TFT_eSPI.h>
   
   TFT_eSPI tft = TFT_eSPI();
   
   void setup() {
     Serial.begin(115200);
     tft.init();
     tft.setRotation(0);
     tft.fillScreen(TFT_RED);
     delay(1000);
     tft.fillScreen(TFT_GREEN);
     delay(1000);
     tft.fillScreen(TFT_BLUE);
     delay(1000);
     tft.fillScreen(TFT_BLACK);
   }
   
   void loop() {
     // Test complete
   }
   ```

3. **Expected Results**
   - Serial monitor should show initialization messages
   - Display should cycle through red, green, blue, then black
   - If display remains black or shows corruption, check wiring

## Troubleshooting Common Issues

### Display Shows Nothing (Black Screen)
- **Check Power**: Verify 3.3V and GND connections
- **Check Reset Pin**: Ensure RESET (D9) is connected
- **Check Library Config**: Verify TFT_eSPI User_Setup.h matches pin assignments

### Display Shows Garbled/Corrupted Image
- **Check SPI Pins**: Verify MOSI (D11) and SCK (D13) connections
- **Check CS and DC**: Ensure CS (D10) and DC (D8) are properly connected
- **Check SPI Speed**: Try reducing SPI frequency in User_Setup.h

### Colors Appear Inverted or Wrong
- **Color Order**: Try changing RGB/BGR setting in User_Setup.h
- **Display Inversion**: Try enabling/disabling TFT_INVERSION_ON

### Backlight Not Working
- **LED Connection**: Verify LED pin connected to D7
- **PWM Capability**: Ensure D7 supports PWM output
- **Brightness Setting**: Check if backlight brightness is set to 0

### Upload/Programming Issues
- **Driver Installation**: Ensure ESP32 board package is installed
- **Board Selection**: Select "Arduino Nano ESP32" in Arduino IDE
- **USB Cable**: Try a different USB-C cable (data-capable)
- **Reset Timing**: Hold BOOT button while uploading if needed

## Safety Considerations

### Electrical Safety
- Always disconnect power before making wiring changes
- Double-check voltage levels (3.3V NOT 5V for LCD)
- Ensure no short circuits between power rails

### Component Protection
- Handle display carefully - LCD panel is fragile
- Avoid static discharge - use anti-static precautions
- Don't exceed current limits - ESP32 GPIO pins max 12mA each

### Workspace Safety
- Use good lighting for precise wiring work
- Keep workspace organized to avoid losing small components
- Have proper ventilation if using flux during soldering

## Next Steps

After successful wiring and testing:

1. **Upload BMO Sketch**: Use the main BMO smiley face program
2. **Test Animations**: Verify blinking and expression changes work
3. **Customize**: Modify expressions, colors, or add new features
4. **Enclosure**: Consider mounting in a BMO-style case
5. **Expansion**: Add buttons, sensors, or sound effects

## Resources

- [Arduino Nano ESP32 Pinout Reference](https://docs.arduino.cc/hardware/nano-esp32)
- [TFT_eSPI Library Documentation](https://github.com/Bodmer/TFT_eSPI)
- [ESP32 SPI Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)
- [Waveshare LCD Module Datasheet](https://www.waveshare.com/wiki/2.4inch_LCD_Module)

## Support

If you encounter issues:
1. Check wiring against this diagram
2. Verify library installation and configuration
3. Test with simple examples before complex code
4. Check serial monitor for error messages
5. Consider voltage measurements with multimeter

Happy building your BMO display project! 🤖✨
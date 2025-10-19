/*
 * TFT_eSPI Library Configuration for BMO Project
 * 
 * This file configures the TFT_eSPI library for use with:
 * - Arduino Nano ESP32
 * - Waveshare 2.4" LCD Module
 * - ILI9341 or ST7789 display controller
 * 
 * IMPORTANT: This file should be placed in the TFT_eSPI library folder:
 * Arduino/libraries/TFT_eSPI/User_Setup.h
 * 
 * Or create a custom setup file and include it in your main sketch
 */

#ifndef _USER_SETUP_H_
#define _USER_SETUP_H_

// BMO PROJECT CONFIGURATION
// =========================

// Define to prevent loading of all the default drivers
#define USER_SETUP_ID 200

// Driver Selection
// ================
// Define the display driver, only one should be defined
#define ILI9341_DRIVER       // Generic driver for common ILI9341 displays
//#define ST7789_DRIVER        // Alternative driver for ST7789 displays

// Display Resolution
// ==================
// For ILI9341 these are typically:
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Pin Configuration for Arduino Nano ESP32
// =========================================
// These pins are defined to match our hardware wiring plan

// SPI Data pins (ESP32 hardware SPI pins)
#define TFT_MISO  12   // GPIO12 - SPI Data In (optional, can be left undefined)
#define TFT_MOSI  11   // GPIO11 - SPI Data Out (required)
#define TFT_SCLK  13   // GPIO13 - SPI Clock (required)

// Control pins (can be any digital pins)
#define TFT_CS    10   // GPIO10 - Chip Select (required)
#define TFT_DC    8    // GPIO8  - Data/Command (required)
#define TFT_RST   9    // GPIO9  - Reset (required)

// Backlight pin (PWM capable pin recommended)
#define TFT_BL    7    // GPIO7  - Backlight control

// SPI Configuration
// =================
// SPI frequency for display writes
#define SPI_FREQUENCY  27000000   // 27MHz - Maximum safe speed for most ESP32/display combinations

// SPI frequency for reading from display (if supported)
#define SPI_READ_FREQUENCY  20000000  // 20MHz - Slower for more reliable reads

// SPI port selection (ESP32 has multiple SPI ports)
// Use HSPI (SPI2) for better performance
#define USE_HSPI_PORT

// Display Features
// ================
// Color order - RGB or BGR depending on display
#define TFT_RGB_ORDER TFT_RGB  // Color order Red-Green-Blue
//#define TFT_BGR_ORDER TFT_BGR  // Alternative: Blue-Green-Red

// Display inversion (uncomment if colors appear inverted)
//#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF

// Font Configuration
// ==================
// Load the font definitions
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel high font, needs ~2666 bytes in FLASH, only numbers 0-9 and : - . /
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel high font, needs ~2438 bytes in FLASH, only numbers 0-9 and : - . /
#define LOAD_FONT8  // Font 8. Large 75 pixel high font needs ~3256 bytes in FLASH, only numbers 0-9 and : - . /
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Smooth font rendering (uses more RAM but looks better)
#define SMOOTH_FONT

// Performance Optimizations
// =========================
// Enable faster SPI writes (ESP32 specific)
#define SPI_HAS_TRANSACTION

// Use DMA for faster transfers (ESP32 specific, experimental)
//#define USE_DMA_TO_TFT

// Double buffering support (requires more RAM)
//#define USE_DMA_TO_TFT

// Memory Optimization
// ===================
// Reduce FLASH usage by excluding unused fonts
//#define LOAD_GLCD   // Comment out fonts you don't need
//#define LOAD_FONT2  // to save FLASH memory
//#define LOAD_FONT4
//#define LOAD_FONT6
//#define LOAD_FONT7
//#define LOAD_FONT8

// Touch Screen Configuration (if applicable)
// ==========================================
// Uncomment if your display has touch functionality
//#define TOUCH_CS  21     // Chip select pin (T_CS) of touch screen
//#define SPI_TOUCH_FREQUENCY  2500000  // Touch controller SPI frequency

// Debug and Development
// =====================
// Comment out to disable debug output
#define DEBUG_TFT_eSPI

// Validate configuration
#if !defined(ILI9341_DRIVER) && !defined(ST7789_DRIVER)
  #error "No display driver defined! Define either ILI9341_DRIVER or ST7789_DRIVER"
#endif

#if defined(ILI9341_DRIVER) && defined(ST7789_DRIVER)
  #error "Multiple display drivers defined! Define only one driver"
#endif

#ifndef TFT_CS
  #error "TFT_CS pin not defined!"
#endif

#ifndef TFT_DC
  #error "TFT_DC pin not defined!"
#endif

#ifndef TFT_RST
  #error "TFT_RST pin not defined!"
#endif

// BMO Project Specific Settings
// ==============================
// These are used by our BMO display code

// Screen orientation (0=portrait, 1=landscape, 2=inverted portrait, 3=inverted landscape)
#define BMO_SCREEN_ROTATION 0

// Default backlight brightness (0-255)
#define BMO_DEFAULT_BRIGHTNESS 200

// Performance settings
#define BMO_ENABLE_ANIMATIONS 1
#define BMO_SMOOTH_RENDERING 1

#endif // _USER_SETUP_H_
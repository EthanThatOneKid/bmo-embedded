/*
 * BMO Display Driver Abstraction Layer
 * 
 * Provides hardware abstraction for the Waveshare 2.4" LCD display
 * Supports both ILI9341 and ST7789 display controllers
 * 
 * Features:
 * - Automatic controller detection
 * - SPI initialization and configuration
 * - Backlight control
 * - Error handling and recovery
 * - Performance optimization
 */

#ifndef BMO_DISPLAY_H
#define BMO_DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Display specifications
#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320
#define DISPLAY_ROTATION 0  // Portrait mode

// Pin definitions (matching hardware wiring plan)
#define TFT_CS    10  // GPIO10 - Chip Select
#define TFT_RST   9   // GPIO9  - Reset
#define TFT_DC    8   // GPIO8  - Data/Command
#define TFT_MOSI  11  // GPIO11 - SPI Data Out
#define TFT_SCK   13  // GPIO13 - SPI Clock
#define TFT_LED   7   // GPIO7  - Backlight Control
#define TFT_MISO  12  // GPIO12 - SPI Data In (optional)

// SPI configuration
#define SPI_FREQUENCY  27000000  // 27MHz - safe speed for ESP32
#define SPI_READ_FREQUENCY 20000000  // Slower for read operations

// Display controller types
enum DisplayController {
  CONTROLLER_UNKNOWN = 0,
  CONTROLLER_ILI9341,
  CONTROLLER_ST7789
};

// Display status codes
enum DisplayStatus {
  DISPLAY_OK = 0,
  DISPLAY_ERROR_INIT,
  DISPLAY_ERROR_SPI,
  DISPLAY_ERROR_CONTROLLER,
  DISPLAY_ERROR_MEMORY
};

class BMODisplay {
public:
  BMODisplay();
  ~BMODisplay();
  
  // Core display functions
  bool begin();
  void end();
  DisplayStatus getStatus() const { return status; }
  DisplayController getController() const { return controller; }
  
  // Backlight control
  void setBacklight(uint8_t brightness);  // 0-255
  uint8_t getBacklight() const { return backlightLevel; }
  void backlightOn() { setBacklight(255); }
  void backlightOff() { setBacklight(0); }
  
  // Display control
  void sleep();
  void wakeup();
  void reset();
  void clear(uint16_t color = 0x0000);
  
  // Performance optimization
  void startWrite();
  void endWrite();
  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  
  // Error handling
  const char* getLastError() const { return lastError; }
  void clearError() { lastError = nullptr; status = DISPLAY_OK; }
  
  // Hardware information
  void printDisplayInfo();
  bool testDisplay();
  
private:
  TFT_eSPI* tft;
  DisplayStatus status;
  DisplayController controller;
  uint8_t backlightLevel;
  const char* lastError;
  bool initialized;
  
  // Initialization helpers
  bool initializeSPI();
  bool detectController();
  bool configureDisplay();
  void initializeBacklight();
  
  // Controller-specific initialization
  bool initILI9341();
  bool initST7789();
  
  // Error handling
  void setError(DisplayStatus errorStatus, const char* message);
  
  // Hardware test functions
  bool testSPIConnection();
  bool testDisplayMemory();
};

// Global display instance (extern declaration)
extern BMODisplay* g_bmoDisplay;

#endif // BMO_DISPLAY_H
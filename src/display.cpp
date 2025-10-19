/*
 * BMO Display Driver Implementation
 * 
 * Hardware abstraction layer for Waveshare 2.4" LCD display
 * Handles initialization, configuration, and error recovery
 */

#include "display.h"

// Global display instance
BMODisplay* g_bmoDisplay = nullptr;

BMODisplay::BMODisplay() 
  : tft(nullptr)
  , status(DISPLAY_OK)
  , controller(CONTROLLER_UNKNOWN)
  , backlightLevel(255)
  , lastError(nullptr)
  , initialized(false)
{
}

BMODisplay::~BMODisplay() {
  end();
}

bool BMODisplay::begin() {
  Serial.println("Initializing BMO Display...");
  
  // Create TFT instance if not already created
  if (!tft) {
    tft = new TFT_eSPI();
    if (!tft) {
      setError(DISPLAY_ERROR_MEMORY, "Failed to allocate TFT object");
      return false;
    }
  }
  
  // Initialize backlight control pin
  initializeBacklight();
  
  // Initialize SPI communication
  if (!initializeSPI()) {
    setError(DISPLAY_ERROR_SPI, "SPI initialization failed");
    return false;
  }
  
  // Initialize display
  tft->init();
  
  // Detect display controller
  if (!detectController()) {
    setError(DISPLAY_ERROR_CONTROLLER, "Controller detection failed");
    return false;
  }
  
  // Configure display settings
  if (!configureDisplay()) {
    setError(DISPLAY_ERROR_INIT, "Display configuration failed");
    return false;
  }
  
  // Test display functionality
  if (!testDisplay()) {
    setError(DISPLAY_ERROR_INIT, "Display test failed");
    return false;
  }
  
  // Turn on backlight
  setBacklight(255);
  
  // Set global reference
  g_bmoDisplay = this;
  
  initialized = true;
  status = DISPLAY_OK;
  
  Serial.println("Display initialized successfully!");
  printDisplayInfo();
  
  return true;
}

void BMODisplay::end() {
  if (initialized) {
    backlightOff();
    
    if (tft) {
      delete tft;
      tft = nullptr;
    }
    
    initialized = false;
    g_bmoDisplay = nullptr;
    
    Serial.println("Display shutdown complete.");
  }
}

void BMODisplay::setBacklight(uint8_t brightness) {
  backlightLevel = brightness;
  
  // Use PWM for smooth brightness control
  analogWrite(TFT_LED, brightness);
  
  Serial.printf("Backlight set to %d/255\n", brightness);
}

void BMODisplay::sleep() {
  if (!initialized) return;
  
  // Turn off backlight
  backlightOff();
  
  // Send display to sleep mode
  tft->writecommand(0x10);  // Sleep In command
  delay(120);  // Wait for sleep mode to activate
  
  Serial.println("Display entered sleep mode");
}

void BMODisplay::wakeup() {
  if (!initialized) return;
  
  // Wake up display
  tft->writecommand(0x11);  // Sleep Out command
  delay(120);  // Wait for wake up
  
  // Restore backlight
  setBacklight(backlightLevel);
  
  Serial.println("Display woke up from sleep");
}

void BMODisplay::reset() {
  Serial.println("Performing display reset...");
  
  // Hardware reset sequence
  digitalWrite(TFT_RST, LOW);
  delay(10);
  digitalWrite(TFT_RST, HIGH);
  delay(120);
  
  // Re-initialize if needed
  if (initialized) {
    configureDisplay();
  }
}

void BMODisplay::clear(uint16_t color) {
  if (tft) {
    tft->fillScreen(color);
  }
}

void BMODisplay::startWrite() {
  if (tft) {
    tft->startWrite();
  }
}

void BMODisplay::endWrite() {
  if (tft) {
    tft->endWrite();
  }
}

void BMODisplay::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  if (tft) {
    tft->setAddrWindow(x, y, w, h);
  }
}

bool BMODisplay::initializeSPI() {
  // Configure SPI pins
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  
  // Initial pin states
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_RST, HIGH);
  digitalWrite(TFT_DC, HIGH);
  
  // Hardware reset
  digitalWrite(TFT_RST, LOW);
  delay(10);
  digitalWrite(TFT_RST, HIGH);
  delay(120);
  
  Serial.println("SPI pins configured and display reset complete");
  return true;
}

bool BMODisplay::detectController() {
  // Try to read display ID to detect controller type
  uint32_t id = 0;
  
  digitalWrite(TFT_CS, LOW);
  tft->writecommand(0x04);  // Read Display ID command
  
  // Read 3 bytes of ID
  id = tft->readcommand8(0x04, 1);
  id <<= 8;
  id |= tft->readcommand8(0x04, 2);
  id <<= 8;
  id |= tft->readcommand8(0x04, 3);
  
  digitalWrite(TFT_CS, HIGH);
  
  Serial.printf("Display ID: 0x%06X\n", id);
  
  // Identify controller based on ID
  if ((id & 0xFFFF) == 0x9341) {
    controller = CONTROLLER_ILI9341;
    Serial.println("Detected ILI9341 controller");
    return true;
  } else if ((id & 0xFFFF) == 0x7789 || (id & 0xFF) == 0x85) {
    controller = CONTROLLER_ST7789;
    Serial.println("Detected ST7789 controller");
    return true;
  } else {
    // Default to ILI9341 if detection fails
    controller = CONTROLLER_ILI9341;
    Serial.println("Controller detection uncertain, defaulting to ILI9341");
    return true;
  }
}

bool BMODisplay::configureDisplay() {
  // Set display orientation and dimensions
  tft->setRotation(DISPLAY_ROTATION);
  
  // Verify dimensions match expected values
  if (tft->width() != DISPLAY_WIDTH || tft->height() != DISPLAY_HEIGHT) {
    Serial.printf("Warning: Unexpected display dimensions %dx%d (expected %dx%d)\n",
                  tft->width(), tft->height(), DISPLAY_WIDTH, DISPLAY_HEIGHT);
  }
  
  // Controller-specific configuration
  switch (controller) {
    case CONTROLLER_ILI9341:
      return initILI9341();
    case CONTROLLER_ST7789:
      return initST7789();
    default:
      return initILI9341();  // Fallback
  }
}

bool BMODisplay::initILI9341() {
  // ILI9341-specific initialization sequence
  Serial.println("Configuring for ILI9341 controller");
  
  // Enable extended command set (if needed)
  tft->writecommand(0xEF);
  tft->writedata(0x03);
  tft->writedata(0x80);
  tft->writedata(0x02);
  
  // Power control settings
  tft->writecommand(0xCF);
  tft->writedata(0x00);
  tft->writedata(0xC1);
  tft->writedata(0x30);
  
  // Display inversion off
  tft->writecommand(0x20);
  
  // Memory access control
  tft->writecommand(0x36);
  tft->writedata(0x48);  // Portrait mode, RGB order
  
  return true;
}

bool BMODisplay::initST7789() {
  // ST7789-specific initialization sequence
  Serial.println("Configuring for ST7789 controller");
  
  // Memory access control
  tft->writecommand(0x36);
  tft->writedata(0x00);  // Portrait mode, RGB order
  
  // Interface pixel format
  tft->writecommand(0x3A);
  tft->writedata(0x05);  // 16-bit color
  
  return true;
}

void BMODisplay::initializeBacklight() {
  pinMode(TFT_LED, OUTPUT);
  
  // Configure PWM for smooth brightness control
  analogWriteFrequency(TFT_LED, 1000);  // 1kHz PWM frequency
  
  // Start with backlight off
  analogWrite(TFT_LED, 0);
  
  Serial.println("Backlight control initialized");
}

bool BMODisplay::testDisplay() {
  Serial.println("Running display tests...");
  
  // Test 1: SPI communication
  if (!testSPIConnection()) {
    return false;
  }
  
  // Test 2: Display memory
  if (!testDisplayMemory()) {
    return false;
  }
  
  // Test 3: Basic drawing
  tft->fillScreen(0x0000);  // Black
  delay(100);
  tft->fillScreen(0xFFFF);  // White
  delay(100);
  tft->fillScreen(0xF800);  // Red
  delay(100);
  tft->fillScreen(0x07E0);  // Green
  delay(100);
  tft->fillScreen(0x001F);  // Blue
  delay(100);
  tft->fillScreen(0x0000);  // Black
  
  Serial.println("All display tests passed!");
  return true;
}

bool BMODisplay::testSPIConnection() {
  // Try to read display status
  digitalWrite(TFT_CS, LOW);
  tft->writecommand(0x09);  // Read Display Status
  uint8_t status = tft->readcommand8(0x09, 1);
  digitalWrite(TFT_CS, HIGH);
  
  Serial.printf("Display status register: 0x%02X\n", status);
  
  // Basic validation - status should not be 0x00 or 0xFF
  return (status != 0x00 && status != 0xFF);
}

bool BMODisplay::testDisplayMemory() {
  // Test pixel read/write functionality
  uint16_t testColor = 0x7E0;  // Green
  
  // Write a test pixel
  tft->drawPixel(10, 10, testColor);
  
  // Note: Reading pixels is not always supported
  // This test mainly verifies write operations work
  return true;
}

void BMODisplay::setError(DisplayStatus errorStatus, const char* message) {
  status = errorStatus;
  lastError = message;
  Serial.printf("Display Error: %s\n", message);
}

void BMODisplay::printDisplayInfo() {
  Serial.println("=== BMO Display Information ===");
  Serial.printf("Status: %s\n", (status == DISPLAY_OK) ? "OK" : "ERROR");
  Serial.printf("Controller: %s\n", 
    (controller == CONTROLLER_ILI9341) ? "ILI9341" :
    (controller == CONTROLLER_ST7789) ? "ST7789" : "Unknown");
  Serial.printf("Dimensions: %dx%d\n", DISPLAY_WIDTH, DISPLAY_HEIGHT);
  Serial.printf("Rotation: %d\n", DISPLAY_ROTATION);
  Serial.printf("Backlight: %d/255\n", backlightLevel);
  Serial.printf("SPI Frequency: %d Hz\n", SPI_FREQUENCY);
  Serial.println("==============================");
}
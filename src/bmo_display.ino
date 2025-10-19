/*
 * BMO Smiley Face Display
 * 
 * Displays a BMO-style smiley face on a Waveshare 2.4" LCD (240x320 pixels)
 * using Arduino Nano ESP32
 * 
 * Hardware:
 * - Arduino Nano ESP32
 * - Waveshare 2.4" LCD Module (ILI9341/ST7789 driver)
 * - SPI connection via breadboard
 * 
 * Pin Connections:
 * LCD Pin    → Arduino Pin    → Function
 * VCC        → 3.3V          → Power supply
 * GND        → GND           → Ground
 * CS         → D10 (GPIO10)  → Chip Select
 * RESET      → D9 (GPIO9)    → Reset
 * DC         → D8 (GPIO8)    → Data/Command
 * MOSI       → D11 (GPIO11)  → SPI Data Out
 * SCK        → D13 (GPIO13)  → SPI Clock
 * LED        → D7 (GPIO7)    → Backlight Control (PWM)
 * MISO       → D12 (GPIO12)  → SPI Data In (optional)
 * 
 * Author: BMO Embedded Project
 * Date: October 19, 2025
 */

#include <TFT_eSPI.h>
#include <SPI.h>
#include "display.h"
#include "graphics.h"

// Display and graphics objects
TFT_eSPI tft = TFT_eSPI();
BMODisplay bmoDisplay;
BMOGraphics bmoGraphics;

// Animation state variables
unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 3000;  // Blink every 3 seconds
bool eyesOpen = true;
int animationPhase = 0;

// BMO color palette (RGB565 format)
#define BMO_TEAL     0x4E6D    // BMO's signature teal color
#define BMO_BLACK    0x0000    // Eyes and mouth
#define BMO_WHITE    0xFFFF    // Eye highlights
#define BMO_DARK_TEAL 0x2945   // Darker teal for shadows

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(100);
  
  Serial.println("BMO Smiley Face Display Starting...");
  
  // Initialize display
  if (!bmoDisplay.begin()) {
    Serial.println("ERROR: Display initialization failed!");
    while (1) {
      delay(1000);
    }
  }
  
  // Initialize graphics system
  bmoGraphics.begin(&tft);
  
  // Draw initial BMO face
  drawBMOFace(true);  // Start with eyes open
  
  Serial.println("BMO is ready! :)");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Handle blinking animation
  if (currentTime - lastBlinkTime >= blinkInterval) {
    eyesOpen = !eyesOpen;
    drawBMOFace(eyesOpen);
    lastBlinkTime = currentTime;
    
    // Randomize next blink interval (2-5 seconds)
    blinkInterval = random(2000, 5000);
    
    Serial.println(eyesOpen ? "Eyes open" : "Blink!");
  }
  
  // Small delay to prevent excessive CPU usage
  delay(50);
}

/**
 * Draw the complete BMO face
 * @param eyesOpen - true for open eyes, false for closed/blinking
 */
void drawBMOFace(bool eyesOpen) {
  // Clear screen with BMO teal background
  tft.fillScreen(BMO_TEAL);
  
  // Calculate face proportions for 240x320 display
  int centerX = 120;  // Half of 240
  int centerY = 160;  // Half of 320
  
  // Eye parameters
  int eyeRadius = 25;
  int eyeY = centerY - 40;
  int leftEyeX = centerX - 50;
  int rightEyeX = centerX + 50;
  
  // Draw eyes
  if (eyesOpen) {
    // Open eyes - black circles with white highlights
    tft.fillCircle(leftEyeX, eyeY, eyeRadius, BMO_BLACK);
    tft.fillCircle(rightEyeX, eyeY, eyeRadius, BMO_BLACK);
    
    // Add eye highlights for shine effect
    tft.fillCircle(leftEyeX - 8, eyeY - 8, 6, BMO_WHITE);
    tft.fillCircle(rightEyeX - 8, eyeY - 8, 6, BMO_WHITE);
  } else {
    // Closed eyes - horizontal lines
    int lineWidth = eyeRadius * 2;
    int lineThickness = 4;
    
    for (int i = 0; i < lineThickness; i++) {
      tft.drawLine(leftEyeX - lineWidth/2, eyeY + i - lineThickness/2, 
                   leftEyeX + lineWidth/2, eyeY + i - lineThickness/2, BMO_BLACK);
      tft.drawLine(rightEyeX - lineWidth/2, eyeY + i - lineThickness/2, 
                   rightEyeX + lineWidth/2, eyeY + i - lineThickness/2, BMO_BLACK);
    }
  }
  
  // Draw mouth - simple curved smile
  drawBMOSmile(centerX, centerY + 30, 60, 20);
  
  // Optional: Add BMO's distinctive rectangular outline
  drawBMOFrame();
}

/**
 * Draw BMO's smile using multiple line segments to create a curve
 * @param centerX - center X position of mouth
 * @param centerY - center Y position of mouth  
 * @param width - width of the smile
 * @param height - height/curve of the smile
 */
void drawBMOSmile(int centerX, int centerY, int width, int height) {
  // Create a curved smile using multiple short line segments
  int segments = 20;
  int startX = centerX - width/2;
  int endX = centerX + width/2;
  int segmentWidth = width / segments;
  
  for (int i = 0; i < segments; i++) {
    int x1 = startX + (i * segmentWidth);
    int x2 = startX + ((i + 1) * segmentWidth);
    
    // Calculate Y positions using a parabolic curve
    float t1 = (float)(x1 - centerX) / (width/2);
    float t2 = (float)(x2 - centerX) / (width/2);
    
    int y1 = centerY + (int)(height * (1 - t1 * t1));
    int y2 = centerY + (int)(height * (1 - t2 * t2));
    
    // Draw thick line segments for visibility
    for (int thickness = 0; thickness < 3; thickness++) {
      tft.drawLine(x1, y1 + thickness, x2, y2 + thickness, BMO_BLACK);
    }
  }
}

/**
 * Draw BMO's rectangular frame outline
 */
void drawBMOFrame() {
  // Draw outer frame border
  int frameThickness = 8;
  int cornerRadius = 15;
  
  // Create rounded rectangle frame effect
  for (int i = 0; i < frameThickness; i++) {
    tft.drawRoundRect(i, i, 240 - (i * 2), 320 - (i * 2), cornerRadius, BMO_DARK_TEAL);
  }
}
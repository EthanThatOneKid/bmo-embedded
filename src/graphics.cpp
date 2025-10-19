/*
 * BMO Graphics Implementation
 * 
 * Implements all BMO-specific drawing functions and animations
 */

#include "graphics.h"

// Global graphics instance
BMOGraphics* g_bmoGraphics = nullptr;

BMOGraphics::BMOGraphics()
  : tft(nullptr)
  , initialized(false)
  , currentExpression(EXPRESSION_HAPPY)
  , currentEyeState(EYES_OPEN)
  , fastDrawMode(false)
  , drawRegionX(0)
  , drawRegionY(0)
  , drawRegionW(240)
  , drawRegionH(320)
{
}

BMOGraphics::~BMOGraphics() {
  end();
}

void BMOGraphics::begin(TFT_eSPI* display) {
  tft = display;
  initialized = (tft != nullptr);
  
  if (initialized) {
    g_bmoGraphics = this;
    Serial.println("BMO Graphics initialized successfully");
  } else {
    Serial.println("ERROR: BMO Graphics initialization failed - no display provided");
  }
}

void BMOGraphics::end() {
  if (initialized) {
    tft = nullptr;
    initialized = false;
    g_bmoGraphics = nullptr;
    Serial.println("BMO Graphics shutdown complete");
  }
}

void BMOGraphics::drawBMOFace(BMOExpression expression, EyeState eyeState) {
  if (!initialized) return;
  
  Serial.printf("Drawing BMO face - Expression: %d, Eyes: %d\n", expression, eyeState);
  
  // Store current state
  currentExpression = expression;
  currentEyeState = eyeState;
  
  // Enable fast drawing mode for complex rendering
  startFastDraw();
  
  // Draw face components in order
  drawBackground();
  drawFrame();
  drawEyes(eyeState);
  drawMouth(expression);
  
  endFastDraw();
}

void BMOGraphics::clearScreen(uint16_t color) {
  if (initialized) {
    tft->fillScreen(color);
  }
}

void BMOGraphics::drawBackground() {
  // Fill entire screen with BMO teal
  tft->fillScreen(BMO_TEAL);
  
  // Add subtle gradient effect (optional)
  for (int y = 0; y < 320; y += 4) {
    uint16_t gradientColor = blendColors(BMO_TEAL, BMO_LIGHT_TEAL, (float)y / 320.0f * 0.1f);
    tft->drawFastHLine(0, y, 240, gradientColor);
  }
}

void BMOGraphics::drawEyes(EyeState state) {
  int leftEyeX = FACE_CENTER_X - EYE_SEPARATION / 2;
  int rightEyeX = FACE_CENTER_X + EYE_SEPARATION / 2;
  int eyeY = FACE_CENTER_Y + EYE_Y_OFFSET;
  
  drawEye(leftEyeX, eyeY, state, true);   // Left eye
  drawEye(rightEyeX, eyeY, state, false); // Right eye
}

void BMOGraphics::drawEye(int centerX, int centerY, EyeState state, bool isLeft) {
  switch (state) {
    case EYES_OPEN:
      // Draw main eye circle
      drawSmoothCircle(centerX, centerY, EYE_RADIUS, BMO_BLACK);
      
      // Add eye highlight for shine effect
      drawEyeHighlight(centerX, centerY);
      break;
      
    case EYES_CLOSED:
      // Draw closed eye as horizontal line
      drawClosedEye(centerX, centerY, EYE_RADIUS * 2);
      break;
      
    case EYES_HALF_CLOSED:
      // Draw half-height oval
      tft->fillEllipse(centerX, centerY, EYE_RADIUS, EYE_RADIUS / 2, BMO_BLACK);
      drawEyeHighlight(centerX, centerY - EYE_RADIUS / 4);
      break;
      
    case EYES_WIDE:
      // Draw larger eyes for surprise
      drawSmoothCircle(centerX, centerY, EYE_RADIUS + 5, BMO_BLACK);
      drawEyeHighlight(centerX, centerY);
      // Add extra highlight
      tft->fillCircle(centerX + 5, centerY - 5, 3, BMO_WHITE);
      break;
  }
}

void BMOGraphics::drawEyeHighlight(int centerX, int centerY) {
  // Main highlight
  tft->fillCircle(centerX - 8, centerY - 8, 6, BMO_WHITE);
  
  // Small secondary highlight
  tft->fillCircle(centerX - 5, centerY - 12, 2, BMO_WHITE);
}

void BMOGraphics::drawClosedEye(int centerX, int centerY, int width) {
  int thickness = 4;
  int halfWidth = width / 2;
  
  // Draw thick horizontal line with rounded ends
  for (int i = 0; i < thickness; i++) {
    tft->drawLine(centerX - halfWidth, centerY + i - thickness/2,
                  centerX + halfWidth, centerY + i - thickness/2, BMO_BLACK);
  }
  
  // Add rounded ends
  tft->fillCircle(centerX - halfWidth, centerY, thickness/2, BMO_BLACK);
  tft->fillCircle(centerX + halfWidth, centerY, thickness/2, BMO_BLACK);
}

void BMOGraphics::drawMouth(BMOExpression expression) {
  int mouthX = FACE_CENTER_X;
  int mouthY = FACE_CENTER_Y + MOUTH_Y_OFFSET;
  
  switch (expression) {
    case EXPRESSION_HAPPY:
      drawHappyMouth(mouthX, mouthY);
      break;
      
    case EXPRESSION_SURPRISED:
      drawSurprisedMouth(mouthX, mouthY);
      break;
      
    case EXPRESSION_SLEEPY:
      drawSleepyMouth(mouthX, mouthY);
      break;
      
    case EXPRESSION_EXCITED:
      drawExcitedMouth(mouthX, mouthY);
      break;
      
    case EXPRESSION_CONFUSED:
      drawConfusedMouth(mouthX, mouthY);
      break;
      
    default:
      drawHappyMouth(mouthX, mouthY);
      break;
  }
}

void BMOGraphics::drawHappyMouth(int centerX, int centerY) {
  // Draw curved smile using multiple line segments
  drawCurve(centerX, centerY, MOUTH_WIDTH, MOUTH_HEIGHT, BMO_BLACK, true);
}

void BMOGraphics::drawSurprisedMouth(int centerX, int centerY) {
  // Draw small oval
  tft->fillEllipse(centerX, centerY, 15, 20, BMO_BLACK);
  // Interior highlight
  tft->fillEllipse(centerX, centerY, 10, 15, BMO_TEAL);
}

void BMOGraphics::drawSleepyMouth(int centerX, int centerY) {
  // Draw small horizontal line
  drawThickLine(centerX - 20, centerY, centerX + 20, centerY, 3, BMO_BLACK);
}

void BMOGraphics::drawExcitedMouth(int centerX, int centerY) {
  // Draw wide open smile with visible "teeth"
  drawCurve(centerX, centerY, MOUTH_WIDTH + 20, MOUTH_HEIGHT + 10, BMO_BLACK, true);
  
  // Add tooth highlights
  for (int i = 0; i < 4; i++) {
    int toothX = centerX - 20 + (i * 13);
    tft->fillRect(toothX, centerY + 8, 3, 8, BMO_WHITE);
  }
}

void BMOGraphics::drawConfusedMouth(int centerX, int centerY) {
  // Draw wavy line
  int segments = 8;
  int segmentWidth = MOUTH_WIDTH / segments;
  
  for (int i = 0; i < segments; i++) {
    int x1 = centerX - MOUTH_WIDTH/2 + (i * segmentWidth);
    int x2 = x1 + segmentWidth;
    int y1 = centerY + ((i % 2 == 0) ? -5 : 5);
    int y2 = centerY + ((i % 2 == 0) ? 5 : -5);
    
    drawThickLine(x1, y1, x2, y2, 2, BMO_BLACK);
  }
}

void BMOGraphics::drawFrame() {
  // Draw BMO's characteristic rectangular border
  int frameThickness = 6;
  int cornerRadius = 12;
  
  // Outer frame
  for (int i = 0; i < frameThickness; i++) {
    drawRoundedRect(i, i, 240 - (i * 2), 320 - (i * 2), cornerRadius, BMO_DARK_TEAL);
  }
  
  // Inner highlight
  drawRoundedRect(frameThickness, frameThickness, 
                  240 - (frameThickness * 2), 320 - (frameThickness * 2), 
                  cornerRadius - 2, BMO_LIGHT_TEAL);
}

void BMOGraphics::animateBlink() {
  if (!initialized) return;
  
  // Close eyes
  drawEyes(EYES_CLOSED);
  delay(BLINK_DURATION);
  
  // Open eyes
  drawEyes(EYES_OPEN);
  
  Serial.println("Blink animation complete");
}

void BMOGraphics::drawSmoothCircle(int centerX, int centerY, int radius, uint16_t color) {
  // Use anti-aliased circle for smooth edges
  drawAntiAliasedCircle(centerX, centerY, radius, color);
}

void BMOGraphics::drawThickLine(int x1, int y1, int x2, int y2, int thickness, uint16_t color) {
  for (int i = 0; i < thickness; i++) {
    for (int j = 0; j < thickness; j++) {
      tft->drawLine(x1 + i - thickness/2, y1 + j - thickness/2,
                    x2 + i - thickness/2, y2 + j - thickness/2, color);
    }
  }
}

void BMOGraphics::drawCurve(int centerX, int centerY, int width, int height, uint16_t color, bool upward) {
  // Draw smooth curve using multiple line segments
  int segments = 20;
  int startX = centerX - width/2;
  int segmentWidth = width / segments;
  
  for (int i = 0; i < segments; i++) {
    int x1 = startX + (i * segmentWidth);
    int x2 = startX + ((i + 1) * segmentWidth);
    
    // Calculate Y positions using parabolic curve
    float t1 = (float)(x1 - centerX) / (width/2);
    float t2 = (float)(x2 - centerX) / (width/2);
    
    int y1 = centerY + (upward ? 1 : -1) * (int)(height * (1 - t1 * t1));
    int y2 = centerY + (upward ? 1 : -1) * (int)(height * (1 - t2 * t2));
    
    drawThickLine(x1, y1, x2, y2, 3, color);
  }
}

void BMOGraphics::drawRoundedRect(int x, int y, int width, int height, int radius, uint16_t color) {
  // Draw rounded rectangle outline
  tft->drawRoundRect(x, y, width, height, radius, color);
}

void BMOGraphics::startFastDraw() {
  if (initialized) {
    tft->startWrite();
    fastDrawMode = true;
  }
}

void BMOGraphics::endFastDraw() {
  if (initialized && fastDrawMode) {
    tft->endWrite();
    fastDrawMode = false;
  }
}

uint16_t BMOGraphics::blendColors(uint16_t color1, uint16_t color2, float ratio) {
  // Extract RGB components
  uint8_t r1 = RED_FROM_565(color1);
  uint8_t g1 = GREEN_FROM_565(color1);
  uint8_t b1 = BLUE_FROM_565(color1);
  
  uint8_t r2 = RED_FROM_565(color2);
  uint8_t g2 = GREEN_FROM_565(color2);
  uint8_t b2 = BLUE_FROM_565(color2);
  
  // Blend components
  uint8_t r = r1 + (uint8_t)((r2 - r1) * ratio);
  uint8_t g = g1 + (uint8_t)((g2 - g1) * ratio);
  uint8_t b = b1 + (uint8_t)((b2 - b1) * ratio);
  
  return RGB565(r, g, b);
}

void BMOGraphics::drawAntiAliasedCircle(int centerX, int centerY, int radius, uint16_t color) {
  // For now, use standard filled circle
  // Anti-aliasing would require more complex pixel blending
  tft->fillCircle(centerX, centerY, radius, color);
  
  // Add subtle outline for smoothness
  tft->drawCircle(centerX, centerY, radius, darkenColor(color, 0.2f));
}

uint16_t BMOGraphics::darkenColor(uint16_t color, float amount) {
  uint8_t r = RED_FROM_565(color);
  uint8_t g = GREEN_FROM_565(color);
  uint8_t b = BLUE_FROM_565(color);
  
  r = (uint8_t)(r * (1.0f - amount));
  g = (uint8_t)(g * (1.0f - amount));
  b = (uint8_t)(b * (1.0f - amount));
  
  return RGB565(r, g, b);
}

void BMOGraphics::printGraphicsInfo() {
  Serial.println("=== BMO Graphics Information ===");
  Serial.printf("Status: %s\n", initialized ? "Initialized" : "Not Initialized");
  Serial.printf("Current Expression: %d\n", currentExpression);
  Serial.printf("Current Eye State: %d\n", currentEyeState);
  Serial.printf("Fast Draw Mode: %s\n", fastDrawMode ? "Active" : "Inactive");
  Serial.println("================================");
}
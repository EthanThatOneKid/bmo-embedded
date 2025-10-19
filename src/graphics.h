/*
 * BMO Graphics Functions
 * 
 * Specialized drawing functions for BMO character elements
 * Optimized for 240x320 TFT display with 16-bit color
 * 
 * Features:
 * - BMO face rendering (eyes, mouth, expressions)
 * - Animation support (blinking, expression changes)
 * - Efficient drawing algorithms
 * - Color palette management
 */

#ifndef BMO_GRAPHICS_H
#define BMO_GRAPHICS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// BMO Color Palette (RGB565 format)
#define BMO_TEAL          0x4E6D    // Primary teal color
#define BMO_DARK_TEAL     0x2945    // Darker shade for shadows/borders
#define BMO_LIGHT_TEAL    0x6EDD    // Lighter shade for highlights
#define BMO_BLACK         0x0000    // Eyes, mouth, outlines
#define BMO_WHITE         0xFFFF    // Eye highlights, teeth
#define BMO_GRAY          0x7BEF    // Neutral elements
#define BMO_BLUE_TINT     0x4E7D    // Slight blue variation

// Face element dimensions (scaled for 240x320 display)
#define FACE_CENTER_X     120       // Horizontal center
#define FACE_CENTER_Y     160       // Vertical center
#define EYE_RADIUS        25        // Standard eye size
#define EYE_SEPARATION    100       // Distance between eye centers
#define EYE_Y_OFFSET      -40       // Eyes above center
#define MOUTH_Y_OFFSET    30        // Mouth below center
#define MOUTH_WIDTH       60        // Standard mouth width
#define MOUTH_HEIGHT      20        // Mouth curve height

// Animation parameters
#define BLINK_DURATION    150       // Milliseconds for blink
#define EXPRESSION_FADE   300       // Milliseconds for expression change

// Expression types
enum BMOExpression {
  EXPRESSION_HAPPY = 0,
  EXPRESSION_SURPRISED,
  EXPRESSION_SLEEPY,
  EXPRESSION_EXCITED,
  EXPRESSION_CONFUSED,
  EXPRESSION_COUNT
};

// Eye states
enum EyeState {
  EYES_OPEN = 0,
  EYES_CLOSED,
  EYES_HALF_CLOSED,
  EYES_WIDE
};

class BMOGraphics {
public:
  BMOGraphics();
  ~BMOGraphics();
  
  // Initialization
  void begin(TFT_eSPI* display);
  void end();
  
  // Main drawing functions
  void drawBMOFace(BMOExpression expression = EXPRESSION_HAPPY, EyeState eyeState = EYES_OPEN);
  void clearScreen(uint16_t color = BMO_TEAL);
  
  // Face components
  void drawBackground();
  void drawEyes(EyeState state);
  void drawMouth(BMOExpression expression);
  void drawFrame();
  
  // Individual eye functions
  void drawEye(int centerX, int centerY, EyeState state, bool isLeft = true);
  void drawEyeHighlight(int centerX, int centerY);
  void drawClosedEye(int centerX, int centerY, int width);
  
  // Mouth variations
  void drawHappyMouth(int centerX, int centerY);
  void drawSurprisedMouth(int centerX, int centerY);
  void drawSleepyMouth(int centerX, int centerY);
  void drawExcitedMouth(int centerX, int centerY);
  void drawConfusedMouth(int centerX, int centerY);
  
  // Animation helpers
  void animateBlink();
  void animateExpressionChange(BMOExpression from, BMOExpression to);
  void fadeTransition(uint16_t fromColor, uint16_t toColor, int steps = 10);
  
  // Utility functions
  void drawSmoothCircle(int centerX, int centerY, int radius, uint16_t color);
  void drawThickLine(int x1, int y1, int x2, int y2, int thickness, uint16_t color);
  void drawCurve(int centerX, int centerY, int width, int height, uint16_t color, bool upward = true);
  void drawRoundedRect(int x, int y, int width, int height, int radius, uint16_t color);
  
  // Color utilities
  uint16_t blendColors(uint16_t color1, uint16_t color2, float ratio);
  uint16_t darkenColor(uint16_t color, float amount);
  uint16_t lightenColor(uint16_t color, float amount);
  
  // Performance optimization
  void startFastDraw();
  void endFastDraw();
  void setDrawRegion(int x, int y, int width, int height);
  
  // Debug and testing
  void drawColorTest();
  void drawGeometryTest();
  void printGraphicsInfo();
  
private:
  TFT_eSPI* tft;
  bool initialized;
  BMOExpression currentExpression;
  EyeState currentEyeState;
  
  // Optimization state
  bool fastDrawMode;
  int drawRegionX, drawRegionY, drawRegionW, drawRegionH;
  
  // Internal drawing helpers
  void drawPixelSafe(int x, int y, uint16_t color);
  bool isInDrawRegion(int x, int y);
  void drawAntiAliasedCircle(int centerX, int centerY, int radius, uint16_t color);
  void drawBezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);
};

// Global graphics instance
extern BMOGraphics* g_bmoGraphics;

// Utility macros
#define RGB565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
#define RED_FROM_565(color) ((color >> 8) & 0xF8)
#define GREEN_FROM_565(color) ((color >> 3) & 0xFC)
#define BLUE_FROM_565(color) ((color << 3) & 0xF8)

#endif // BMO_GRAPHICS_H
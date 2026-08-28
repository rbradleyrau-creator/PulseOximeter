/**
 * @file graphics.c
 * @brief c file for graphic expansion of ST7735 provided display functions
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 */

#include <graphics.h>

// Constants
// Heart Pixels
const uint16_t HEART_X[] = {3,4,5,6,7,14,15,16,17,18,2,3,4,5,6,7,8,13,14,15,16,17,18,19,1,2,3,4,5,6,7,8,9,12,13,14,15,16,17,18,19,
                      20,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,0,1,2,
                      3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,1,2,3,4,5,
                      6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,2,3,4,5,6,7,8,9,10,11,
                      12,13,14,15,16,17,18,19,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,4,5,6,7,8,9,10,11,12,13,14,15,16,17,5,6,7,8,9,10,11,
                      12,13,14,15,16,6,7,8,9,10,11,12,13,14,15,7,8,9,10,11,12,13,14,8,9,10,11,12,13,9,10,11,12,10,
                      11};
const uint16_t HEART_Y[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
                      2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
                      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 
                      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 
                      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 
                      10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 
                      12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 
                      15, 15, 16, 16, 16, 16, 17, 17};
const int HEART_PXL_CNT = 258;   

// Bolt (charging icon) Pixels
const uint16_t BOLT_X[] = {3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6,
                      0, 1, 2, 3, 4, 5, 6, 3, 4, 5, 6, 3, 4, 5, 2, 3, 4, 2, 3, 1, 2, 1};
const uint16_t BOLT_Y[] = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7,
                      7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14};
const int BOLT_PXL_CNT = 57;

// Variable Initializations
int prevHR = 80;        // Any non-zero value works at initialization
float prevSP02 = 98.0f; // Any non-zero value works at initialization
bool chargeIconActive = false;

/**
 * Draws a heart (22 x 18) at a User specified location
 *
 * @param x x-axis start location
 * @param y y-axis start location
 * @param color Color of the image
 */
void ST7735_DrawHeart(uint16_t x, uint16_t y, uint16_t color) {

  // Draws a heart by looping though the pixel value in the array
  for (int i = 0; i < HEART_PXL_CNT; i++) {
    ST7735_DrawPixel(x + HEART_X[i], y+ HEART_Y[i], color);
  }

}

/**
 * Draws a Bolt (7 x 15) at a User specified location
 *
 * @param x x-axis start location
 * @param y y-axis start location
 * @param color Color of the image
 */
void ST7735_DrawBolt(uint16_t x, uint16_t y, uint16_t color) {

  // Draws a bolt by looping though the pixel value in the array
  for (int i = 0; i < BOLT_PXL_CNT; i++) {
    ST7735_DrawPixel(x + BOLT_X[i], y+ BOLT_Y[i], color);
  }

}

/**
 * Refreshes the screen with updated values
 *
 * @param hr most recent heart rate reading
 * @param y most recent sp02 reading
 * @param firstInit true if this is the first time the screen is updated
 */
void updateScreen(int hr, float sp02, bool firstInit) {
  ST7735_SetRotation(0); // sets vertical text rotation

  // Variable Declarations
  // char redLEDStr[8], irLEDStr[8], greenLEDStr[8], ambStr[8], ratioStr[8]; // #FIXME Testing strings

  if (firstInit) {
    // Clear screen
    fillScreen(BACKGROUND_MAIN);

    // Set initial Display
    ST7735_WriteString(17, 2, "HR", Font_11x18, SOFT_RED,BACKGROUND_MAIN);
    ST7735_DrawHeart(41, 2, SOFT_RED);
    ST7735_WriteString(7, 52, "Sp02 %", Font_11x18, SOFT_GREEN,BACKGROUND_MAIN);

    // Draw Sub-section
    fillRect(0, 133, 80, 27, BACKGROUND_SECONDARY);
    drawLine(0, 133, 79, 133, SOFT_BLUE);

    // Draw Battery Icon
    fillRect(17, 142, 5, 11, SOFT_ORANGE); // Battery head
    fillRect(22, 138, 55, 19, SOFT_ORANGE);
    fillRect(25, 141, 49, 13, BACKGROUND_MAIN);

    for (int i = 0; i < 4; i++) {
      fillRect(26 + (12 * i), 142, 11, 11, SOFT_ORANGE);
    }

  }

  // Checks if Heartrate has changed, if so, update on screen
  if (prevHR != hr) {
    char hrStr[4];

    // Update HR
    if (hr != 0) {
      sprintf(hrStr, "%03d", hr);
    } else {
      sprintf(hrStr, "---");
    }

    // Update screen
    ST7735_SetRotation(0); // sets default text rotation
    fillRect(15, 24, 48, 26, BACKGROUND_MAIN);
    ST7735_WriteString(15, 24, hrStr, Font_16x26, WHITE,BACKGROUND_MAIN);

    prevHR = hr;
  }

  // Checks if Sp02 has changed, if so, update on screen
  if (prevSP02 != sp02) {
    char sp02Str[7];

    if (sp02 >= 100.0f) {
      // Display 100%
      sprintf(sp02Str, "100%%");
    } else if (sp02 != 0.0f) {
      // %03.1f is not supported, so a workarounds is used
      int leftDot = (int) sp02;
      int rightDot = (int) (10 * sp02) % 10;
      sprintf(sp02Str, "%02d.%1d", leftDot, rightDot);
    } else {
      sprintf(sp02Str, "----");
    }

    // Update screen
    ST7735_SetRotation(0); // sets default text rotation
    fillRect(7, 74, 64, 26, BACKGROUND_MAIN);
    ST7735_WriteString(7, 74, sp02Str, Font_16x26, WHITE,BACKGROUND_MAIN);

    prevSP02 = sp02;
  }

  // Check for USB connection, is so, so charging
  if (!chargeIconActive && USB_IsPluggedIn()) {
    ST7735_DrawBolt(5, 140, SOFT_YELLOW);
    chargeIconActive = true;
  }
  else if (chargeIconActive && !USB_IsPluggedIn()) {
    fillRect(5, 140, 7, 15, BACKGROUND_SECONDARY);
    chargeIconActive = false;
  }

  // #FIXME testing Values displayed in lower portion of screen 
  // sprintf(redLEDStr, "%07d", (int) red_pd1);
  // sprintf(irLEDStr, "%07d", (int) ir_pd1);
  // sprintf(greenLEDStr, "%07d", (int) green_pd1);
  // sprintf(ambStr, "%07d", (int) ambient_pd1);
  // sprintf(ratioStr, "%07d", (int) green_pd1 / (int) ambient_pd1);

  // Clear previous numbers
  // fillRect(2, 101, 77, 18, BACKGROUND_MAIN);
  // fillRect(2, 121, 77, 18, BACKGROUND_MAIN);
  // fillRect(2, 141, 77, 18, BACKGROUND_MAIN);

  // // Write down exact red, ir, and green values
  // ST7735_WriteString(2, 101, greenLEDStr,   Font_11x18, GREEN, BACKGROUND_MAIN);
  // ST7735_WriteString(2, 121, ambStr,    Font_11x18, SOFT_GREEN, BACKGROUND_MAIN);
  // ST7735_WriteString(2, 141, ratioStr, Font_11x18, RED, BACKGROUND_MAIN);
}

void ST7735_LoadingBar(void) {

  fillScreen(BACKGROUND_MAIN);

  // Create loading bar with logo 
  fillRect(15, 25, 130, 30, WHITE);
  fillRect(20, 30, 120, 20, BACKGROUND_SECONDARY);
  ST7735_WriteString(16, 4, PRGM_NAME,   Font_11x18, WHITE, BACKGROUND_MAIN);
  HAL_Delay(LOAD_DELAY);

  // LOADING BAR (for visual effect)
  // LOADING PHASE 1
  fillRect(20, 30, 20, 20, SOFT_RED);
  HAL_Delay(LOAD_DELAY);

  // LOADING PHASE 2
  fillRect(40, 30, 20, 20, SOFT_ORANGE);
  HAL_Delay(LOAD_DELAY);

  // LOADING PHASE 3
  fillRect(60, 30, 20, 20, SOFT_YELLOW);
  HAL_Delay(LOAD_DELAY);

  // LOADING PHASE 4
  fillRect(80, 30, 20, 20, SOFT_GREEN);
  HAL_Delay(LOAD_DELAY);

  // LOADING PHASE 5
  fillRect(100, 30, 20, 20, SOFT_BLUE);
  HAL_Delay(LOAD_DELAY);

  // LOADING PHASE 6
  fillRect(120, 30, 20, 20, SOFT_PURPLE);
  HAL_Delay(LOAD_DELAY);

  // Set to Homescreen
  HAL_Delay(LOAD_DELAY / 3);
}

// Powers down the device (device will turn off when STM32 enters shutdown mode)
void ST7735_Shutdown() {
  fillScreen(BLACK);
}
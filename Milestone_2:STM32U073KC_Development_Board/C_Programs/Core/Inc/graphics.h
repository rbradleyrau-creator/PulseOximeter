/**
 * @file graphics.h
 * @brief Header file for graphic expansion of ST7735 Driver provided by Controllers Tech. 
 *        Files: 'ST7735.h', 'GFX_FUNCTION.h', & 'fonts.h' are protected via licence and
 *        can only be accessed from Controllers Tech's website linked below.
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 *
 * @see https://controllerstech.com/terms-conditions/
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <stdio.h>
#include "ST7735.h"
#include "GFX_FUNCTIONS.h"
#include "stm32u0xx_hal.h"

// Cosmetic defenitions
#define PRGM_NAME "G.Cole OS" 
#define LOAD_DELAY 150

// Additional color defenitions (5-6-5)
#define	SOFT_RED    0xF2CB
#define	SOFT_ORANGE 0xFCA7
#define	SOFT_YELLOW 0xFEC6
#define	SOFT_GREEN  0x36E6
#define	SOFT_BLUE   0x631D
#define	SOFT_PURPLE 0xDBFA
#define BACKGROUND_MAIN      0x859F // Main background color (light blue)
#define BACKGROUND_SECONDARY 0x64DE // Seconadary background color for detailing (slightly darker light blue)

// constant declarations
extern const uint16_t HEART_X[];
extern const uint16_t HEART_Y[];
extern const int HEART_PXL_CNT;
extern const uint16_t BOLT_X[];
extern const uint16_t BOLT_Y[];
extern const int BOLT_PXL_CNT;

extern int prevHR;
extern float prevSP02;
extern bool chargeIconActive;

// Function prototypes
void ST7735_DrawHeart(uint16_t x, uint16_t y, uint16_t color);
void ST7735_DrawBolt(uint16_t x, uint16_t y, uint16_t color);

void updateScreen(int hr, float sp02, bool firstInit); 
void ST7735_LoadingBar(void);

void ST7735_Shutdown(void);

// external required functions
extern bool USB_IsPluggedIn(void);

#endif /* GRAPHICS_H */

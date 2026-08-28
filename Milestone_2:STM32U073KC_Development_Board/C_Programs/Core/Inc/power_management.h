/**
 * @file power_management.h
 * @brief Header file for functions pertaining to device power usage
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 */

#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "stm32u0xx_hal.h"
#include <stdbool.h>

#define USB_DETECTION_PORT       GPIOB
#define USB_DETECTION_PIN        GPIO_PIN_3
#define PWR_BTN_GPIO_Port        GPIOA          
#define PWR_BTN_GPIO_Pin         GPIO_PIN_2 
#define PWR_WKP_PIN_POL PWR_WAKEUP_PIN4_LOW   
#define PWR_WKP_PIN_FLAG PWR_FLAG_WUF4

#define BTN_ACTIVE_LOW       1    // Button actives on low read
#define DEBOUNCE_MS          30
#define PWR_OFF_MS           800  // Time pressed before Power off

// Function definitions
bool USB_IsPluggedIn(void);
bool Pwr_Btn_IsPressed(void);
void Pwr_Btn_CheckForPress(void);
void EnterShutdown(void);

// external required functions
extern void MAX86141_Shutdown(void);
extern void ST7735_Shutdown(void);

#endif
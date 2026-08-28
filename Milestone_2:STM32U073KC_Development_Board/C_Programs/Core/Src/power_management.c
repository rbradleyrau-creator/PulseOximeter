/**
 * @file power_management.c
 * @brief c file for functions pertaining to device power usage
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 */

#include <power_management.h>

/**
 * Checks for USB Power
 *
 * @return true if USB is detected
 */
bool USB_IsPluggedIn(void)
{
    return (HAL_GPIO_ReadPin(USB_DETECTION_PORT, USB_DETECTION_PIN) == GPIO_PIN_SET);
}

/**
 * Checks for Button trigger
 *
 * @return true if button is pressed
 */
bool Pwr_Btn_IsPressed(void)
{
  GPIO_PinState raw = HAL_GPIO_ReadPin(PWR_BTN_GPIO_Port, PWR_BTN_GPIO_Pin);
  #if BTN_ACTIVE_LOW
    return (raw == GPIO_PIN_RESET);
  #else
    return (raw == GPIO_PIN_SET);
  #endif
}


 /*
  * Checks how long the power button is pressed and performs functions accordingly
  */
void Pwr_Btn_CheckForPress(void)
{
    if (!Pwr_Btn_IsPressed()) {
        return;
    }

    HAL_Delay(DEBOUNCE_MS);
    if (!Pwr_Btn_IsPressed()) {
        return; // was noise/bounce
    }

    uint32_t start = HAL_GetTick();
    while (Pwr_Btn_IsPressed()) {
        if ((HAL_GetTick() - start) >= PWR_OFF_MS) {
            EnterShutdown();
        }
    }
    return; // released before PWR_OFF_MS
}

/*
 * Shuts down the Device
 */
void EnterShutdown(void)
{
    // Make sure the button is actually released before we try to sleep,
    // otherwise WFI sees a pending wakeup event and falls straight through.
    while (Pwr_Btn_IsPressed()) {
        HAL_Delay(10);
    }
    HAL_Delay(50); // extra settle time for contact bounce on release

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    HAL_PWR_EnableWakeUpPin(PWR_WKP_PIN_POL);

    if (__HAL_PWR_GET_FLAG(PWR_WKP_PIN_FLAG) == RESET) {
        MAX86141_Shutdown();
        ST7735_Shutdown();
        HAL_PWR_EnterSHUTDOWNMode();
        // Shutdown only if flag succesfully cleared
    }
}
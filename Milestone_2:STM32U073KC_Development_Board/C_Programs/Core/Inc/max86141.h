/**
 * @file max86141.h
 * @brief Header file for Max86141 HR/Sp02 sensor integration
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 *
 * @see https://www.analog.com/media/en/technical-documentation/data-sheets/max86140-max86141.pdf
 */

/*
 * max86141.h
 *
 * Driver for the MAX86141 optical AFE, configured for GREEN-LED HEART
 * RATE ONLY with direct ambient sampling (Figure 2 in the datasheet).
 *
 * Board LED wiring: LED1 = Red, LED2 = IR, LED3 = Green.
 * Heart rate uses the green LED, so this driver drives LED3.
 *
 * Target: STM32U073KCU6, using the hardware SPI1 peripheral.
 *
 * Pin map (as configured in CubeMX):
 *   PA4 = CS   (plain GPIO output, NOT SPI1_NSS -- software-controlled
 *               so a single CS-low pulse can span a multi-byte burst
 *               FIFO read)
 *   PA6 = MISO (SPI1_MISO, AF)
 *   PA7 = MOSI (SPI1_MOSI, AF)
 *   PA1 = SCLK (SPI1_SCK, AF)
 *
 * SPI1 must be configured in CubeMX as: Full-Duplex Master, 8-bit data,
 * MSB first, CPOL = Low, CPHA = 1 Edge (SPI Mode 0), Hardware NSS
 * disabled, prescaler chosen to keep SCLK <= 4 MHz.
 */

#ifndef MAX86141_H
#define MAX86141_H

#include "stm32u0xx_hal.h"
#include <stdint.h>

// SPI configurations
#define MAX_CS_PORT     GPIOA
#define MAX_CS_PIN      GPIO_PIN_4
#define MAX_SPI_TIMEOUT_MS 100

/* Register addresses & Configuration bits */
#define MAX86141_WRITE 0x00
#define MAX86141_READ 0x80

#define MAX86141_REG_INT_STATUS1        0x00 // correct
#define MAX86141_REG_INT_STATUS2        0x01 // correct
#define MAX86141_REG_INT_ENABLE1        0x02 // correct
#define MAX86141_REG_INT_ENABLE2        0x03 // correct

#define MAX86141_REG_FIFO_WR_PTR        0x04 
#define MAX86141_REG_FIFO_RD_PTR        0x05 
#define MAX86141_REG_OVF_COUNTER        0x06
#define MAX86141_REG_FIFO_DATA_COUNT    0x07
#define MAX86141_REG_FIFO_DATA          0x08
#define MAX86141_REG_FIFO_CONFIG1       0x09
#define MAX86141_REG_FIFO_CONFIG2       0x0A 
    // FIFO_CONFIG2 bit positions
    #define MAX86141_FIFO_CFG2_FLUSH_FIFO    (1 << 4)
    #define MAX86141_FIFO_CFG2_FIFO_STAT_CLR (1 << 3)
    #define MAX86141_FIFO_CFG2_A_FULL_TYPE   (1 << 2)
    #define MAX86141_FIFO_CFG2_FIFO_RO       (1 << 1)

#define MAX86141_REG_SYSTEM_CTRL        0x0D 
    // System Control bit positions
    #define MAX86141_SYSCTRL_SINGLE_PPG     (1 << 3)
    #define MAX86141_SYSCTRL_LP_MODE        (1 << 2)
    #define MAX86141_SYSCTRL_SHDN           (1 << 1)
    #define MAX86141_SYSCTRL_RESET          (1 << 0)

#define MAX86141_REG_PPG_SYNC_CTRL      0x10 
#define MAX86141_REG_PPG_CONFIG1        0x11 
    // Register 0x11 layout: [ALC_DISABLE][ADD_OFFSET][PPG2_ADC_RGE 5:4][PPG1_ADC_RGE 3:2][PPG_TINT 1:0] (Pgs 55 & 68)
    // ADC current ranges (0x0 = ~4uA, 0x1 = ~8uA, 0x2 = ~16uA, 0x3 = ~32uA)
    // Pulse width/integration time (0x0 = 15.8us, 0x1 = 29.4us, 0x2 = 58.7us, 0x3 = 117.3us)
    #define ALC_DISABLE  0x0 // Enabled
    #define ADD_OFFSET   0x0 // No offset
    #define PPG2_ADC_RGE 0x2 // 16uA range
    #define PPG1_ADC_RGE 0x2 // 16uA range
    #define PPG_TINT     0x3 // 117.3us integration time
#define MAX86141_REG_PPG_CONFIG2        0x12 
    // Register 0x12 layout: [PPG_SR 7:3][SMP_AVE 3:0] (Pg 55)
    // Refer to Page 69 of datasheet for sample rates
    // Refer to Page 70 of datasheet for sample averaging
    // Note: True sample rate = PPG_SR/SMP_AVE (Ex: 128 sps (base)/2 avg = 64 sps)
    #define PPG_SR       0x0E // 128 sps
    #define SMP_AVE      0x01 // 2 averaged 
#define MAX86141_REG_PPG_CONFIG3        0x13 
    // Register 0x13 layout: [LED_SETLNG 7:6][DIG_FILT_SEL][-][-][BURST_RATE 2:1][BURST_EN] (Pg 55)
    // LED Settling (0x0 = 4us, 0x1 = 6us, 0x2 = 8us, 0x3 = 12us) (Pg 71)
    // Digital Filter Selection (0x0 = CDM, 0x1 = FDM) (Pg 72)
    // Burst Rate (0x0 = 8Hz, 0x1 = 32Hz, 0x2 = 84Hz, 0x3 = 256Hz) (Pg 72)
    // Burst En (0x0 = Disabled, 0x1 = Enabled) (Pg 72)
    #define LED_SETLNG   0x3 // 12 us
    #define DIG_FILT_SEL 0x1 // FDM
    #define BURST_RATE   0x0 // 8 Hz
    #define BURST_EN     0x0 // Disabled

#define MAX86141_REG_PROX_INT_THRESH    0x14 
#define MAX86141_REG_PHOTO_DIODE_BIAS   0x15 
    // Register 0x15 layout: [-][PDBIAS2 6:4][-][PDBIAS1 2:0] (Pg. 55)
    // Photodiode Bias capacitance (0x1 = 0-65pF, 0x5 = 65-130pF, 0x6 = 130-260pF, 0x7 = 260-520pF) (Pg 73)
    #define PDBIAS1 0x1 // 0-65pF
    #define PDBIAS2 0x1 // 0-65pF
#define MAX86141_REG_PICKET_FENCE       0x16 

// Registers used for sequencing LED flashes (Table 1 & 2 in datasheet, Pg 15) 
#define MAX86141_REG_LED_SEQ1           0x20 
#define MAX86141_REG_LED_SEQ2           0x21 
#define MAX86141_REG_LED_SEQ3           0x22 
    // LED Sequence register defenitions
    #define LEDC1 0x1 // Pulse LED1 (Red)
    #define LEDC2 0x2 // Pulse LED2 (IR)
    #define LEDC3 0x3 // Pulse LED3 (Green)
    #define LEDC4 0x0 // No pulse
    #define LEDC5 0x0 // No pulse
    #define LEDC6 0x0 // No pulse

#define MAX86141_REG_LED1_PA            0x23  /* Red */
#define MAX86141_REG_LED2_PA            0x24  /* IR */
#define MAX86141_REG_LED3_PA            0x25  /* Green */
    // LED CURRENTS (mA) (DO NOT EXCEED SELECTED RANGE)
    #define LED1_CURR 2.5f  // Red
    #define LED2_CURR 5.0f  // IR
    #define LED3_CURR 15.0f // Green
#define MAX86141_REG_LED_RANGE1         0x2A
    // Register 0x2A layout: [–][–][LED3_RGE 5:4][LED2_RGE 3:2][LED1_RGE 1:0]
    // LED current ranges (0x0 = 31mA, 0x1 = 62mA, 0x2 = 93 mA, 0x3 = 124 mA)
    #define LED1_RGE 0x0 // 31 mA
    #define LED2_RGE 0x0 // 31 mA
    #define LED3_RGE 0x1 // 62 mA

// Define the 8-bit register value for LED currents based on the chosen LED range
#define LED1_CURR_VAL (uint8_t) (LED1_CURR / (31 * ((int) LED1_RGE + 1)) * 256)
#define LED2_CURR_VAL (uint8_t) (LED2_CURR / (31 * ((int) LED2_RGE + 1)) * 256)
#define LED3_CURR_VAL (uint8_t) (LED3_CURR / (31 * ((int) LED3_RGE + 1)) * 256)

#define MAX86141_REG_PART_ID            0xFF

/* FIFO tag values we care about (Table 3 in datasheet, Page 16) */
#define MAX86141_TAG_PPG1_LEDC1   0x01  /* PPG1, Red LED exposure       */
#define MAX86141_TAG_PPG1_LEDC2   0x02  /* PPG1, IR LED exposure        */
#define MAX86141_TAG_PPG1_LEDC3   0x03  /* PPG1, Green LED exposure     */
// #define MAX86141_TAG_PPG1_LEDC4   0x04  /* PPG1, direct ambient sample  */
#define MAX86141_TAG_PPG2_LEDC1   0x07  /* PPG2, Red LED exposure       */
#define MAX86141_TAG_PPG2_LEDC2   0x08  /* PPG2, IR LED exposure        */
#define MAX86141_TAG_PPG2_LEDC3   0x09  /* PPG2, Green LED exposure     */
// #define MAX86141_TAG_PPG2_LEDC4   0x0A  /* PPG2, direct ambient sample  */
#define MAX86141_TAG_INVALID      0x1E  /* attempted read of empty FIFO */


// grab externally defined spi
extern SPI_HandleTypeDef hspi1;

/* One decoded FIFO sample */
typedef struct {
    uint8_t  tag;     /* 5-bit value, left-justified */
    uint32_t value;   /* 19-bit ADC value, right-justified */
} MAX86141_Sample;

typedef void (*MAX86141_SampleCallback)(const MAX86141_Sample *sample);

// Ensure MX_SPI1_Init() is called prior to MAX86141_INIT
void     MAX86141_Init(void);
void     MAX86141_Poll(MAX86141_SampleCallback cb);

void     MAX86141_WriteReg(uint8_t addr, uint8_t data);
uint8_t  MAX86141_ReadReg(uint8_t addr);
void     MAX86141_ReadFifoBurst(uint8_t *buf, uint16_t numBytes);

void     MAX86141_Shutdown(void);

#endif /* MAX86141_H */

/**
 * @file max86141.c
 * @brief c file for Max86141 HR/Sp02 sensor integration
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 */

#include "max86141.h"

// SPI help functions (Used to toggle the CS line)
static inline void MAX86141_Select(void)  { HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_RESET); }
static inline void MAX86141_Unselect(void) { HAL_GPIO_WritePin(MAX_CS_PORT, MAX_CS_PIN, GPIO_PIN_SET);   }

// SPI communication wrapper
static void max_spi_txrx(const uint8_t *tx, uint8_t *rx, uint16_t len)
{
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)tx, rx, len, MAX_SPI_TIMEOUT_MS);
}

/* ------------------------------------------------------------------ */
/* Register read/write                                                */
/* ------------------------------------------------------------------ */

/**
 * Single-register write
 *
 * @param adrr 8-bit register address to write to
 * @param data 8-bit data to be written to chosen address
 */
void MAX86141_WriteReg(uint8_t addr, uint8_t data)
{
    uint8_t tx[3] = { addr, MAX86141_WRITE, data };
    uint8_t rx[3];

    MAX86141_Select();
    max_spi_txrx(tx, rx, 3);
    MAX86141_Unselect();
}

/**
 * Single-register read
 *
 * @param adrr 8-bit register address to read from
 * @return returns the 8-bit data contained in the chosen register
 */
uint8_t MAX86141_ReadReg(uint8_t addr)
{
    uint8_t tx[3] = { addr, MAX86141_READ, 0x00 };
    uint8_t rx[3];

    MAX86141_Select();
    max_spi_txrx(tx, rx, 3);
    MAX86141_Unselect();

    return rx[2];
}

/**
 * FIFO burst read: Clocks out numBytes of FIFO data in one continuous transmission
 *
 * @param buf buffer to burst read to
 * @param numBytes number of bytes to burst read (valid numbers only, no error checking included)
 */
void MAX86141_ReadFifoBurst(uint8_t *buf, uint16_t numBytes)
{
    uint8_t header_tx[2] = { MAX86141_REG_FIFO_DATA, MAX86141_READ };
    uint8_t header_rx[2];

    MAX86141_Select();
    max_spi_txrx(header_tx, header_rx, 2);

    /* Clock out dummy bytes while reading the FIFO payload */
    for (uint16_t i = 0; i < numBytes; i++) {
        buf[i] = 0x00;
    }
    max_spi_txrx(buf, buf, numBytes);

    MAX86141_Unselect();
}

/* ------------------------------------------------------------------ */
/* Full init sequence                                                  */
/* ------------------------------------------------------------------ */
void MAX86141_Init(void)
{
    MAX86141_Select(); 

    // Soft reset
    MAX86141_WriteReg(MAX86141_REG_SYSTEM_CTRL, MAX86141_SYSCTRL_RESET);
    HAL_Delay(10); // Datasheet requested delay following a reset

    // clear interupts, if any
    MAX86141_ReadReg(MAX86141_REG_INT_STATUS1);
    MAX86141_ReadReg(MAX86141_REG_INT_STATUS2);

    // Put MAX86141 in shutdown so it doesn't operate whilst its configured
    MAX86141_WriteReg(MAX86141_REG_SYSTEM_CTRL, MAX86141_SYSCTRL_SHDN);

    // PPG Configurations
    MAX86141_WriteReg(MAX86141_REG_PPG_CONFIG1, (ALC_DISABLE << 7) | (ADD_OFFSET << 6) | (PPG2_ADC_RGE << 4)
                                                        | (PPG1_ADC_RGE << 2) | PPG_TINT);
    MAX86141_WriteReg(MAX86141_REG_PPG_CONFIG2, PPG_SR << 3 | SMP_AVE); 
    MAX86141_WriteReg(MAX86141_REG_PPG_CONFIG3, (LED_SETLNG << 6) | (DIG_FILT_SEL << 5) | (BURST_RATE << 1) | BURST_EN);

    // Set photodiode Bias
    MAX86141_WriteReg(MAX86141_REG_PHOTO_DIODE_BIAS, (PDBIAS2 << 4) | PDBIAS1);

    // Set current ranges
    MAX86141_WriteReg(MAX86141_REG_LED_RANGE1, (LED3_RGE << 4) | (LED2_RGE << 2) | LED1_RGE); 

    // Setting Drive current
    MAX86141_WriteReg(MAX86141_REG_LED1_PA, LED1_CURR_VAL); 
    MAX86141_WriteReg(MAX86141_REG_LED2_PA, LED2_CURR_VAL); 
    MAX86141_WriteReg(MAX86141_REG_LED3_PA, LED3_CURR_VAL); 

    // Set LED Sequence
    MAX86141_WriteReg(MAX86141_REG_LED_SEQ1, (LEDC2 << 4) | LEDC1);
    MAX86141_WriteReg(MAX86141_REG_LED_SEQ2, (LEDC4 << 4) | LEDC3);
    MAX86141_WriteReg(MAX86141_REG_LED_SEQ3, (LEDC6 << 4) | LEDC5);

    // Configure the Fifo 
    // FIFO_RO is configured so that new data overides old data in the event of an overflow
    MAX86141_WriteReg(MAX86141_REG_FIFO_CONFIG2,
                       MAX86141_FIFO_CFG2_FIFO_RO | MAX86141_FIFO_CFG2_FLUSH_FIFO); // Flush
    MAX86141_WriteReg(MAX86141_REG_FIFO_CONFIG2, MAX86141_FIFO_CFG2_FIFO_RO); // Clear Flush

    // Start Sampling in Low Power mode (Also clears Shutdown)
    MAX86141_WriteReg(MAX86141_REG_SYSTEM_CTRL, MAX86141_SYSCTRL_LP_MODE);

    MAX86141_Unselect();
}

/* ------------------------------------------------------------------ */
/* Polling FIFO reader                                                  */
/* ------------------------------------------------------------------ */

 /**
 * Polls the MAX86141 for new data
 *
 * @param cb function called on newly recieved data
 */
void MAX86141_Poll(MAX86141_SampleCallback cb)
{
    uint8_t ovf   = MAX86141_ReadReg(MAX86141_REG_OVF_COUNTER);
    uint8_t count = MAX86141_ReadReg(MAX86141_REG_FIFO_DATA_COUNT);

    uint16_t numSamples = (ovf != 0) ? 128 : count;
    if (numSamples == 0) {
        return;
    }

    /* Read in reasonably sized chunks to keep the stack buffer small.
     * 32 samples * 3 bytes = 96 bytes per chunk. */
    uint8_t buf[32 * 3];

    while (numSamples > 0) {
        uint16_t chunk = (numSamples > 32) ? 32 : numSamples;

        MAX86141_ReadFifoBurst(buf, chunk * 3);

        for (uint16_t i = 0; i < chunk; i++) {
            uint8_t b0 = buf[i * 3 + 0];
            uint8_t b1 = buf[i * 3 + 1];
            uint8_t b2 = buf[i * 3 + 2];

            MAX86141_Sample sample;
            // Grab 5-bit left-justified tag and 19-bit right justified value
            sample.tag   = (b0 >> 3) & 0x1F;
            sample.value = (((uint32_t)(b0 & 0x07)) << 16) |
                           (((uint32_t)b1) << 8) |
                           ((uint32_t)b2);

            if (cb != NULL && sample.tag != MAX86141_TAG_INVALID) {
                cb(&sample);
            } 
        }

        numSamples -= chunk;
    }
}

/*
 * Shutsdown the MAX86141 device safely
 */
void MAX86141_Shutdown(void) {
    MAX86141_WriteReg(MAX86141_REG_SYSTEM_CTRL, MAX86141_SYSCTRL_SHDN);
}
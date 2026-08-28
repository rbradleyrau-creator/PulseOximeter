/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUFFER_SIZE 512 // Number of samples before peaks are computed (10*128)
// We toss out the first second so 128 + 896 = 1024
#define BUFFER_SAMPLE_START 32
#define HP_FILTER_ALPHA 0.975f
#define LP_FILTER_ALPHA 0.840f
#define FINGER_THRESHOLD_MIN 20000.0f

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
static volatile uint32_t red_pd1        = 0;
static volatile uint32_t ir_pd1         = 0;
static volatile uint32_t green_pd1      = 0;
static volatile uint32_t red_pd2        = 0;
static volatile uint32_t ir_pd2         = 0;
static volatile uint32_t green_pd2      = 0;

// Variable declarations/intializations
int rBuffer[BUFFER_SIZE] = {0}, iBuffer[BUFFER_SIZE] = {0};
float gBuffer[BUFFER_SIZE] = {0.0f};
int bufferIdx = 0, hr = 0;
float sp02 = 0.0f, ratio = 0.0f;

float hp_prevInput_grn = 0.0f, hp_prevOutput_grn = 0.0f, lp_prevOutput_grn = 0.0f;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);

/* USER CODE BEGIN PFP */
// static float movingAverage(float *array, float newVal, int idx, int numTerms);
static int isFingerDetected();
static void PERIPHERAL_Init(); // initializes all peripherals
static void updateHR();
static void updateSP02();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void on_sample(const MAX86141_Sample *s)
{
    switch (s->tag) {
        case MAX86141_TAG_PPG1_LEDC1: red_pd1 = s->value; break;     // PPG1 Red
        case MAX86141_TAG_PPG1_LEDC2: ir_pd1 = s->value; break;      // PPG1 IR
        case MAX86141_TAG_PPG1_LEDC3: green_pd1 = s->value; break;   // PPG1 Green
        case MAX86141_TAG_PPG2_LEDC1: red_pd2 = s->value; 
          // Update red buffer
          if (bufferIdx < BUFFER_SIZE) { // Out of Range protection
            rBuffer[bufferIdx] = ((int) red_pd1 + (int) red_pd2) / 2;
          }
        break;     // PPG2 Red
        case MAX86141_TAG_PPG2_LEDC2: ir_pd2 = s->value; 
          // Update IR buffer
          if (bufferIdx < BUFFER_SIZE) { // Out of Range protection
            iBuffer[bufferIdx] = ((int) ir_pd1 + (int) ir_pd2) / 2;
          }
        break;      // PPG2 IR
        case MAX86141_TAG_PPG2_LEDC3: green_pd2 = s->value;
          // Update green buffer
          if (bufferIdx < BUFFER_SIZE) {
            float avgGreen = ((float) green_pd1 + (float) green_pd2) / 2;
            float hpFiltered = highPassFilter(avgGreen, &hp_prevInput_grn, &hp_prevOutput_grn, HP_FILTER_ALPHA);
            float lpFiltered = lowPassFilter(hpFiltered, &lp_prevOutput_grn, LP_FILTER_ALPHA);
            // gBuffer[bufferIdx] = movingAverage(gBuffer, lpFiltered, bufferIdx, 6);
            gBuffer[bufferIdx] = lpFiltered;

            bufferIdx++; // increment all buffers (green is collected last)
          }
          // Check to see if all sample have been collected for this interval
          if (bufferIdx >= BUFFER_SIZE) {
            updateHR();
            updateSP02();
            bufferIdx = 0; // set all buffer indices to zero
          }
        break;   // PPG2 Green
        default: break; 
    }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  // Initialize all peripherals
  PERIPHERAL_Init();

  // Variable declarations/intializations
  int loopCnt = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // Grab latest sample
    MAX86141_Poll(on_sample);

    // 1000 / 80 is 12.5
    // 1000 / 128 ~= 8 (< 8)
    HAL_Delay(12);

    Pwr_Btn_CheckForPress();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Only updates once per second
    if (loopCnt >= FS/2) {

      loopCnt = 0;

      if (!isFingerDetected()) {
        // Reset data collection until finger is detected again.
        bufferIdx = 0;
        hr = 0;
        sp02 = 0.0f;
        resetIntervalBuffer();
      } 

      updateScreen(hr, sp02, false);
      
    }
    loopCnt++;

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /*Configure GPIO pins : PA2 */
  GPIO_InitStruct.Pin  = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;      // see note below
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

static int isFingerDetected() {

  float currGreen = ((float) green_pd1 + (float) green_pd2) / 2;

  if (currGreen > FINGER_THRESHOLD_MIN) {
    return 1;
  }

  return 0;
}

// static float movingAverage(float *array, float newVal, int idx, int numTerms) {

//   // Value to return
//   float val = 0.0f;

//   // stash value in array
//   array[idx] = newVal;

//   // compute the mean
//   if (idx < numTerms) {
//     val = mean(array, idx + 1);
//   } else {
//     val = mean(array + (idx - numTerms + 1), numTerms);
//   }

//   return val;

// }

static void PERIPHERAL_Init() {
  // NOTE: All delays are for visual effect

  // Ensure that the both CS bits are high before initialization
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_SET); 

  // Initialize LCD display (with ST7735 drivers)
  ST7735_Init(1);

  // Initialize max86141
  MAX86141_Init();

  // Display Loading bar for visual effect
  ST7735_LoadingBar();
  updateScreen(hr, sp02, true);

}

static void updateHR() {
  findPeaks(gBuffer + BUFFER_SAMPLE_START, BUFFER_SIZE - BUFFER_SAMPLE_START);
  hr = (int) heartRate();
}

static void updateSP02() {
  calculate_SpO2(rBuffer, iBuffer, BUFFER_SIZE, &sp02, &ratio);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

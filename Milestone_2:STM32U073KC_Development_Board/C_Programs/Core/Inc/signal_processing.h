/**
 * @file signal_processing.h
 * @brief Header file for signal processing functions utilized in obtaining heart rate and
 *        Sp02 readings from the raw MAX86141 (or other sensor) signals
 *        Note: The following code is adapted from previously created programs written by Mohsen
 *        PC & Mario Regus, who adapted their code from Raivis Strogonovs's MAX30100 program. Both 
          Repositories are linked below.
          Changes include:
            + MAX86141 functionality
            + Low pass filter function
            - Process PPG Signals function
            + Moving Average Function
            + Heart rate history using previously found intervals
 * @author Ryan Rau
 * @version 1.0
 * @date August 27th, 2026
 *
 * @see https://github.com/MohsenAnvariBio/heartrate-spo2-stm32/tree/main 
 * @see https://github.com/xcoder123/MAX30100/tree/master
 */

#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include "stm32u0xx_hal.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define FS 64          // Sampling frequency (True frequency from MAX86141)
#define INTERVAL_HIST_LENGTH 15
#define MAX_R_PEAKS 100

/* 
 * Algorithm based on IEEE paper: A Real-Time QRS Complex Detector Based on Adaptive Thresholding 
 * (https://ieeexplore.ieee.org/abstract/document/7138573).
 *
 * Defenitions below pertain to the above mentioned algorithm
 * At FS = 128, recommended settings are N=3, Nd=2, &PTh = 5.4
 */
#define DERIV_DIST 2  // (Nd)
#define MOV_AVG_AMT 3 // (N)
#define PTH 2.7f

#define RR_MIN  (int)(0.25 * FS)
#define QRS_INT (int)(0.10 * FS)

extern float intervalBuffer[INTERVAL_HIST_LENGTH];
extern int intervalIdx;
extern uint32_t R[];
extern uint32_t rCount;

// Function Prototypes
float mean(float *array, int length);
float median(float *array, int count);
float highPassFilter(float input, float *prevInput, float *prevOutput, float alpha);
float lowPassFilter(float input, float *prevOutput, float alpha);
void movingAverage(float *array, int length, int numTerms);

void findPeaks(float *dataBuffer, int length);
uint16_t heartRate(void);
void addInterval(float newVal);
void resetIntervalBuffer();

void calculate_SpO2(int *redSignal, int *irSignal, int length, float *SpO2, float *ratio);


#endif /* SIGNAL_PROCESSING_H */

/*
 * @file signal_processing.c
 * @brief c file for signal processing functions utilized in obtaining heart rate and
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

#include <signal_processing.h>

// Variable initializations
float intervalBuffer[INTERVAL_HIST_LENGTH] = {0.0f};
int intervalIdx = 0;
uint32_t R[MAX_R_PEAKS] = {0};
uint32_t rCount = 0;

/**
 * Calculate the mean of an array.
 *
 * @param array Pointer to the array of floats
 * @param length Number of elements in the array
 * @return The calculated mean of the array
 */
float mean(float *array, int length) {
	float sum = 0;
	for (int i = 0; i < length; i++) {
		sum += array[i];
	}
	return sum / length;
}

/**
 * Calculate the median of an array.
 *
 * @param array Pointer to the array of floats
 * @param count Number of elements in the array
 * @return The calculated median of the array
 */
float median(float *array, int count) {
    // Sort the array
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (array[i] > array[j]) {
                // Swap elements
                float temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    // Find and return the median
    if (count % 2 == 0) {
        // Even number of elements: median is average of the two middle elements
        return (array[count / 2 - 1] + array[count / 2]) / 2.0f;
    } else {
        // Odd number of elements: median is the middle element
        return array[count / 2];
    }
}

/**
 * Apply a high-pass filter to a signal.
 *
 * @param input Current signal input
 * @param prevInput Pointer to the previous input value
 * @param prevOutput Pointer to the previous output value
 * @param alpha Filter coefficient
 * @return The filtered signal output
 */
float highPassFilter(float input, float *prevInput, float *prevOutput, float alpha) {
    float inputF = input; // No need to cast, input is already float (done so that prevInput points to somethign that wont be deleted)
    float output = alpha * (*prevOutput + inputF - *prevInput);
    *prevInput = inputF;
    *prevOutput = output;
    return output;
}

/**
 * Apply a low-pass filter to a signal.
 *
 * @param input Current signal input
 * @param prevOutput Pointer to the previous output value
 * @param alpha Filter coefficient
 * @return The filtered signal output
 */
float lowPassFilter(float input, float *prevOutput, float alpha) {

    float output = (alpha * *prevOutput) + ((1-alpha) * input);
    *prevOutput = output;
    return output;

}

/**
 * Apply a low-pass filter to a signal.
 *
 * @param array array to be averaged
 * @param length length of the array
 * @param numTerms max amount to average for each given point
 */
void movingAverage(float *array, int length, int numTerms) {

    // Variable Declarations/initializations
    int idx = 0;

    // compute the mean
    if (idx < numTerms) {
        array[idx] = mean(array, idx + 1);
        idx++;
    } else {
        array[idx] = mean(array + (idx - numTerms + 1), numTerms);
        idx++;
    }

}


/**
 * Detect peaks in the data buffer to identify R-peaks.
 *
 * @param dataBuffer Pointer to the input signal array
 * @param length Number of samples in the dataBuffer
 * @param R Pointer to an array to store detected R-peak indices
 * @param R_count Pointer to store the number of detected R-peaks
 */
void findPeaks(float *dataBuffer, int length) {

	// Temporary buffers
	float filtData[length - DERIV_DIST];
	float max_val[length / RR_MIN];

    // Implement pre-processing algorithm
    // Compute Derivative
    for (int i = 0; i < length - DERIV_DIST; i++) {
		filtData[i] = dataBuffer[i + DERIV_DIST] - dataBuffer[i]; 
	}

    // Find the moving average
    movingAverage(filtData, length - DERIV_DIST, MOV_AVG_AMT);

    // Find the Squares
	for (int i = 0; i < length - DERIV_DIST; i++) {
		filtData[i] = (filtData[i] * filtData[i]);
	}

	// Dynamic threshold and peak detection
	float th = 100; // Initial threshold
	int n = 0, i = 0;
	rCount = 0; // Initialize R-peak count

	while (n < length - DERIV_DIST) {
		if (filtData[n] > th) {
			float local_max = 0;
			int local_max_pos = 0;

			// Find local maximum in the window
			for (int k = 0; k < RR_MIN + QRS_INT && n + k < length - DERIV_DIST; k++) {
				if (filtData[n + k] > local_max) {
					local_max = filtData[n + k];
					local_max_pos = k;
				}
			}

			// Store the peak information
			max_val[i] = local_max;
			R[i] = n + local_max_pos + 2;
			rCount++;

			// Update indices and threshold
			int d = RR_MIN + QRS_INT - local_max_pos;
			n += RR_MIN + QRS_INT + RR_MIN - d;
			th = mean(max_val, i + 1);
			i++;
		} else {
            // Decrease threshold as spike are not found
			th *= exp(-PTH / (float)FS);
			n++;
		}
	}
}

/**
 * Calculate heart rate from detected R-peak indices.
 *
 * @param R Pointer to the array of R-peak indices
 * @param R_count Number of detected R-peaks
 * @return Calculated heart rate in beats per minute (BPM), or 0 on error
 */
uint16_t heartRate(void) {

    // Determine if the number of datapoints is valid for the given sample rate
    if (rCount < 5 || rCount > 36) {
        return 0; // Return 0 if insufficient data
    }

    // Calculate RR intervals
    int interval = 0;
    for (int i = 0; i < rCount - 1; i++) {
        interval = R[i + 1] - R[i];
        // Check if the interval is valid given the current sample rate
        // Add to definitions, but the +/- 4 is to allow for wiggle room at the fast heartbeats
        // In fact all of these will eventually be replaced by defenitions with explanations eventually
        if (16 - 4 <= interval && interval <= 128 + 4) {
            addInterval(interval);
        }
    }

    // Calculate median of RR intervals
    float medianRR = median(intervalBuffer, intervalIdx);

    // Check for division by zero 
    if (medianRR == 0) {
        return 0;
    }

    // Calculate and return heart rate
    uint16_t HR = (uint16_t)((60 * FS) / medianRR);
    return HR;
}

/**
 * Adds a new value to the heart rate interval array
 *
 * @param newVal value to add to the array
 */
void addInterval(float newVal) {
    
    if (intervalIdx < INTERVAL_HIST_LENGTH) {
        intervalBuffer[intervalIdx] = newVal;
        intervalIdx++;
    } 
    else {
        // Shift all values down by one
        for (int i = 0; i < INTERVAL_HIST_LENGTH; i++) {
            intervalBuffer[i] = intervalBuffer[i+1];
        }
        // Add in new value to last location
        intervalBuffer[INTERVAL_HIST_LENGTH - 1] = newVal;
    }

}

// Resets Heart rate intervals
void resetIntervalBuffer() {
    // Resets interval Idx to zero
    intervalIdx = 0;
}

/**
 * Calculate SpO2 from red and IR data.
 *
 * @param redBuffer Array of red light PPG data
 * @param irBuffer Array of infrared light PPG data
 * @param length Number of samples in each buffer
 * @param SpO2 Pointer to store the calculated SpO2 value
 * @param ratio Pointer to store the calculated ratio (optional)
 */
void calculate_SpO2(int *redSignal, int *irSignal, int length, float *SpO2, float *ratio) {
    float acRed = 0, acIr = 0, rSig = 0, iSig = 0;

    // Calculate the RMS of the signals (AC component)
    for (int i = 0; i < length; i++) {
        rSig = (float) redSignal[i];
        iSig = (float) irSignal[i];
        acRed += rSig * rSig;
        acIr += iSig * iSig;
    }
    acRed = sqrt(acRed / length);
    acIr = sqrt(acIr / length);

    // Calculate the ratio of AC components
    *ratio = acRed / acIr;

    // Estimate SpO2 using the ratio
    *SpO2 = 110.0 - 25.0 * (*ratio);  // Adjust coefficients as needed
}
 
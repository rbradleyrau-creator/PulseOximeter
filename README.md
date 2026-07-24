# Personal Pulse Oximeter

This repository contains the schematics, designs, and other information regarding the design of a personal Pulse Oximeter. As part of the design process, the overal pulse oximeter design was broken up into three smaller milestones to allow for easier testing and prototyping before moving on to a final design. More information on each of these milestones and the final design can be found via the links below.

## Milestone 1: SpO2 & HR Sensor 

  This milestone involves creating a breakout board that isolates the sensor to be used in the final system. The implementation includes an Optical Pulse Oximeter/Heart-Rate Sensor and a linear voltage regular connected to headers. Through this, an external processor can be programmed to interface with the sensor and testing can be performed to determine the best layout in the final design. This board will also be used to write firmware for the final design. 
  
## Milestone 2: STM32UO73KC Development Board 

  In this milestone, the pins of an STM32UO73KCU6 is connected to various headers for use in testing. In addition to the processor, the PCB also includes USB 2.0, 3.3V Buck Converter, and optional Battery power/charging. The goal of this milestone is to begin writing programs to the STM32 via USB/SWD for use in the final design and to determine the efficiency in of various batteries in powering the system.

  [Read More](https://github.com/rbradleyrau-creator/PulseOximeter/tree/main/Milestone_2%3ASTM32U073KC_Development_Board)

## Milestone 3: Display

  The main goal of this milestone is to write firmware for the STM32 that displays information to an IPS LCD Display. To begin writing this firmware, a 13P FFC breakout board was developed to allow the previously designed development board to interface with the chosen display.

  [Read More](https://github.com/rbradleyrau-creator/PulseOximeter/tree/main/Milestone_3%3ALCD_Display)

# Final Design

  The final design for this project will include an SpO2/HR sensor connected to an STM32. The STM32 will read data from the sensor and display it to the chosen display. The system will incorporated a battery for wireless use with a USB port for charging. The system will also enter deep sleep when not in use which is controlled via an ON/OFF switch. The display will feature both the heartrate readings, SpO2 readings, and the remaining battery life. All the previously mentioned parts are combined into a single, double-sided PCB and encased inside a 3D printed enclosure. This enclosure will include padding just below the sensor along with an adjustable band allowing the device to be mounted to a finger.

# (DT010ATFT) LCD Display

  The final design will feature heartrate, SpO2, and battery life readings on an LCD Display. The goal of this milestone is to: A) create a breakout board that allows the chosen display to interface with the microcontroller developed in the previous milestone, and B) develop firmware for displaying the output of various sensors. To begin this process, a breakout board was made that converts a 13 position FFC (flexible flat cable) into a breakout of 13 pins, plus one extra for shield connection. This board can be seen below.
<br> <br>
  The final step of this process required that LCD libraries be installed and utilized to display the intended data. To do this, research had to be conducted to find suitable libraries and learn their implementations. Once this information was finalized, the library would be implemented into a firmware program that will be used in the final design. 
<br> <br>
  As of July 23rd, 2026, this milestone is still a work in progress.

## 13P FFC Breakout Board

<div align="center">
  <div style="overflow-x: auto; gap: 10px; padding-bottom: 10px; white-space: nowrap; display: inline-block; margin-right: 10px;">
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_3%3ALCD_Display/Drawings%2BSchematics/13P_FFC_Breakout_Topside.png" width="200" height="180" />
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_3%3ALCD_Display/Drawings%2BSchematics/13P_FFC_Breakout_TopLayer.png" width="200" height="180" />
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_3%3ALCD_Display/Drawings%2BSchematics/13P_FFC_Breakout_Bottomside.png" width="200" height="180" />
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_3%3ALCD_Display/Drawings%2BSchematics/13P_FFC_Breakout_BottomLayer.png" width="200" height="180" />
  </div>
</div>

## List of Major Components/Datasheets

- [LCD Display - DT010ATFT](https://www.mouser.com/datasheet/3/3687/1/DT010ATFT-Rev1_1-Displaytech-Spec.pdf) <br>
- [2x7 Surface Mounted Pins - 15910140](https://www.molex.com/en-us/products/part-detail-pdf/15910140?display=pdf) <br>
- [13P FFC Connector - 527451397](https://www.molex.com/en-us/products/part-detail-pdf/527451397?display=pdf) <br>

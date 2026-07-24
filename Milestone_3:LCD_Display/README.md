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

- [STM32U073KCU6](https://www.st.com/resource/en/datasheet/stm32u073c8.pdf) <br>
- [Buck Converter - LM3676](https://www.ti.com/lit/ds/symlink/lm3676.pdf?ts=1784331233319&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM3676) <br>
- [Battery Charger IC - TPB4056B](https://static.3peak.com/res/doc/ds/Datasheet_TPB4056B.pdf) <br>
- [Schottky Diode - NSR0620P2](https://www.onsemi.com/pdf/datasheet/nsr0620p2-d.pdf) <br>
- [P-Channel MOSFET - DMG1013T](https://www.diodes.com/datasheet/download/DMG1013T.pdf) <br>
- [ESD Diodes - USBLC6-2SC6Y](https://www.st.com/resource/en/datasheet/usblc6-2sc6y.pdf) <br>
- [USB Connector - 2171790001](https://www.molex.com/en-us/products/part-detail/2171790001?display=pdf&utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API?utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API) <br>
- [Battery Connector - S2B-PH-SM4-TB](https://www.jst-mfg.com/product/pdf/eng/ePH.pdf) <br>

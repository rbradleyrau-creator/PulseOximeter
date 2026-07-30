# MAX86141 Breakout Board
<div align="center">
  <div style="overflow-x: auto; gap: 10px; padding-bottom: 10px; white-space: nowrap; display: inline-block; margin-right: 10px;">
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_3D_Topside.png" width="400" height="180" />
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_3D_BottomSide.png" width="400" height="180" />
  </div>
  <br>
  
  <details>
    <summary><b>Click here to expand Copper Layers</b></summary>
    <br>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_TopLayer.png" width="100%">
    <hr>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_InLayer1.png" alt="Inner Layer 1" width="100%">
    <hr>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_InLayer2.png" alt="Inner Layer 2" width="100%">
    <hr>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_BottomLayer.png" alt="Bottom Layer" width="100%">
  </details>
</div>

  In order to test and write firmware for the MAX86141 HR/SpO2 sensor, a breakout board was created. The board includes the MAX86141 itself, along with 1 LED emitter emitting infrared, 
  red, and green light, and 2 photodiodes for interpreting the reflections of these light waves. In addition to that, a 1.8V linear regulator is included to power the sensor. 
<br> <br>
  The board can be interfaced with via the pin headers on the left-hand side. The 3.3V pin provides power to the aforementioned LDO, LED, and LED drivers (in the MAX86141) and 
  the Ground pin provided a common ground between the internal and external systems. The top 4 pins (CSB, SDI, SDO, SCLK) provide access to the sensor's readings via SPI.

## List of Major Components/Datasheets

- [MAX86141]([https://www.st.com/resource/en/datasheet/stm32u073c8.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/max86140-max86141.pdf)) <br>
- [LDO - LP5907](https://www.ti.com/lit/ds/symlink/lp5907.pdf) <br>
- [LED(s) - SFH 7018B](https://www.mouser.com/datasheet/3/5912/1/SFH_7018B_EN.pdf) <br>
- [Photodiode - SFH 2704](https://www.mouser.com/datasheet/3/5912/1/SFH_2704_EN.pdf) <br>

***

### [Return to Project Page](https://github.com/rbradleyrau-creator/PulseOximeter/tree/main) <br>

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

- [STM32U073KCU6](https://www.st.com/resource/en/datasheet/stm32u073c8.pdf) <br>
- [Buck Converter - LM3676](https://www.ti.com/lit/ds/symlink/lm3676.pdf?ts=1784331233319&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM3676) <br>
- [Battery Charger IC - TPB4056B](https://static.3peak.com/res/doc/ds/Datasheet_TPB4056B.pdf) <br>
- [Schottky Diode - NSR0620P2](https://www.onsemi.com/pdf/datasheet/nsr0620p2-d.pdf) <br>
- [P-Channel MOSFET - DMG1013T](https://www.diodes.com/datasheet/download/DMG1013T.pdf) <br>
- [ESD Diodes - USBLC6-2SC6Y](https://www.st.com/resource/en/datasheet/usblc6-2sc6y.pdf) <br>
- [USB Connector - 2171790001](https://www.molex.com/en-us/products/part-detail/2171790001?display=pdf&utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API?utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API) <br>
- [Battery Connector - S2B-PH-SM4-TB](https://www.jst-mfg.com/product/pdf/eng/ePH.pdf) <br>

***

### [Return to Project Page](https://github.com/rbradleyrau-creator/PulseOximeter/tree/main) <br>

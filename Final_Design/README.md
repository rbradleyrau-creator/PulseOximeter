# Final Design
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
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_InLayer1.png" alt="SInner Layer 1" width="100%">
    <hr>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_InLayer2.png" alt="Inner Layer 2" width="100%">
    <hr>
    <img src="https://github.com/rbradleyrau-creator/PulseOximeter/blob/main/Milestone_2%3ASTM32U073KC_Development_Board/Drawings%2BSchematics/STM32U073_BottomLayer.png" alt="Bottom Layer" width="100%">
  </details>
</div>

### Project Overview & Features

The final design consists of two major parts:
  - A PCBA containing each critical component
  - A 3D Modeled Enclosure to house the PCBA and other electronics

  The PCB for the pulse oximeter combines all of the previous features into a singular board, with the STC3115AIQT as an exception. The board consists of an STM32U073KC processor connected to three peripherals: the STC3115AIQT fuel gauge over I2C, the MAX86141 HR/Sp02 sensor over SPI1, and an FFC connector over SPI3 for interfacing with the DT010ATFT LCD display. In addition, the processor is also connected to USB 2.0 and has SWD, boot, and reset access via exposed plated through holes. Aside from those connections, the board operates on 3.3V which is provided by the LM3676 buck converter. This component is supplied with anywhere from 5.5V (USB) to 3.0V (Battery), all of which are valid voltages for the LM3676 and a large factor for why the part was chosen. The board also includes MCP73831T battery charger which charges the battery whilst the USB is connected. 
  <br><br>
  The enclosure that houses this PCB measures at 3cm wide, 6cm long, and approximately 2cm tall and is designed for 3D printing. It is separated into two pieces that contain four dowels for a secure connection, however, the strength of this connection varies from printer to printer. In addition, the enclosure provides four openings in its casing: one at the bottom for the LED and photodiodes of the sensor, one at the top for the LCD Display, one on the side for the ON/OFF button, and one in the rear for USB port access. 

### Design Process

  The main schematic design of the PCB is largely a combination of the previous three boards into one singular board, however with some exceptions. Firstly, the TPB4056B battery charger was replaced with the MCP73831T. This was due to the fact that the TPB took up significant area on the PCB and was no longer available at the selected PCB manufacturer. A second change was the inclusion of the STC3115AIQT fuel gauge. This part was included in order to better gauge the battery life of the attached 3.7V Li-ion battery, since the discharge curve of Li-ion batteries are largely non-linear. One final design decision was the inclusion of plated through-holes for debugging. The decision was made in order to debug the newly included component and to avoid additional costs incurred due to additional manufacturing. 
  <br><br>
	For layout, the design was based around a centralized STM32 with the external connectors (USB, PH connector, ON/OFF switch, etc) being positioned in a way that made sense with the intended device enclosure. The board stack up was 4 layers with the top and bottom layers acting as signal layers and the internal layers as ground planes (with a few exceptions). One major design challenge that came up in the making of this board was the 3.3V plane that existed on the bottom of the board. Due to the large number of interconnections, this plane was broken up in several areas leading to vias being omitted from the plane or being connected by thin copper polygons. To fix this issue, components were moved to areas where they could source higher current in addition to a bridge through layer 2 to connect two un-connected polygons in the plane.
  <br><br>
	A second challenge in the design came from the placement of the LM3676 buck converter in relation to the USB data lines. Due to the required placement of the PH connector, the buck converter was located in the bottom left corner. To prevent the high-frequency switching from affecting sensitive components,  sensitive ICs like the MAX86141 IC were placed in the opposite corner of the board. However, the data lines of the USB could not be relocated. To minimize the effect this switching would have on these lines, the STM32 was shifted upward to allow the maximum distance between the data lines and the offending trace. This distance ended up being 5 times larger than the trace width of the data lines, providing satisfactory protection for full-speed USB.
  <br><br>
	A third challenge that came up in the design was the routing of SPI. Originally, the board was designed to operate with only one MOSI & MISO. However, this would have required a star connection scheme, resulting in disconnected ends causing reflections and ringing. To remove this factor, tertiary SPI pins on the STM32 were used to set up two separate communications. While this did take up more real estate on the board, it did allow for the display and MAX86141 connections to operate at different clocking frequencies which allowed the refresh rate of the DT010ATFT to quadruple. 

### Testing and Results

 As of 9/26/2026, the board is still in manufacturing and will be programmed and validated upon arrival. 

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

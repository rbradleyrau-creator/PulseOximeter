# STM32U073KC Development Board

  This PCB Design utilizes an STM32U073KCU6 as its central processor. The board involes a USB header which enables Full Speed USB 2.0 communication with ESD protection and filtered 5 volts of potential throughout the board. This voltage is connected to an LM3676 buck converter that steps the voltage down to 3.3V. Additionally, the 5V provided by a USB connection is connected to a TPB4056B Battery Charging IC to charge a connected battery. Power for the board can also (optionally) be provided via a battery. For this design, a 3.7V Li-Polymer battery is recommended. A connected battery can continue to provide power to the system up until the voltage accross its terminals reaches a minimum of 3.0V. However, while the STM32 will continue to operate at this voltage, care should be taken to ensure that connected peripherals can also operate at this voltage. 
<br> <br>
  Power to the system will be provided via USB if one is connected, otherwise it will be provided via the connected battery. This feature is achieved by using a Schottky diode in addition to a P-channel MOSFET. Once the USB is connected, the 5V provided to the MOSFET's Gate will prevent current flow from the source to the drain, essentially disconnecting the battery from the buck converter. When the USB is disconnected, backflow to the 5V line (which is now grounded) is prevented via the previously mentioned diode.
<br> <br>
  Other features include: Boot and Reset buttons and LED indicators for charging, charge standby, and USB connection. 

## List of Major Components/Datasheets

- [STM32U073KCU6](https://www.st.com/resource/en/datasheet/stm32u073c8.pdf) <br>
- [Buck Converter - LM3676](https://www.ti.com/lit/ds/symlink/lm3676.pdf?ts=1784331233319&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM3676) <br>
- [Battery Charger IC - TPB4056B](https://static.3peak.com/res/doc/ds/Datasheet_TPB4056B.pdf) <br>
- [Schottky Diode - NSR0620P2](https://www.onsemi.com/pdf/datasheet/nsr0620p2-d.pdf) <br>
- [P-Channel MOSFET - DMG1013T](https://www.diodes.com/datasheet/download/DMG1013T.pdf) <br>
- [ESD Diodes - USBLC6-2SC6Y](https://www.st.com/resource/en/datasheet/usblc6-2sc6y.pdf) <br>
- [USB Connector - 2171790001](https://www.molex.com/en-us/products/part-detail/2171790001?display=pdf&utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API?utm_source=M2X&utm_medium=api&utm_campaign=api&utm_id=M2X_API) <br>
- [Battery Connector - S2B-PH-SM4-TB](https://www.jst-mfg.com/product/pdf/eng/ePH.pdf) <br>



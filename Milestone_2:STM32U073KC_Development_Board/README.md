# STM32U073KC Development Board

  This PCB Design utilizes an STM32U073KCU6 as its central processor. The board involes a USB header which enables Full Speed USB 2.0 communication with ESD protection and filtered 5 volts of potential throughout the board. This voltage is connected to an LM3676 buck converter that steps the voltage down to 3.3V. Additionally, the 5V provided by a USB connection is connected to a TPB4056B Battery Charging IC to charge a connected battery. Power for the board can also (optionally) be provided via a battery. For this design, a 3.7V Li-Polymer battery is recommended. A connected battery can continue to provide power to the system up until the voltage accross its terminals reaches a minimum of 3.0V. However, while the STM32 will continue to operate at this voltage, care should be taken to ensure that connected peripherals can also operate at this voltage. 
<br> <br>
  Power to the system will be provided via USB if one is connected, otherwise it will be provided via the connected battery. This feature is achieved by using a Schottky diode in addition to a P-channel MOSFET. Once the USB is connected, the 5V provided to the MOSFET's Gate will prevent current flow from the source to the drain, essentially disconnecting the battery from the buck converter. When the USB is disconnected, backflow to the 5V line (which is now grounded) is prevented via the previously mentioned diode.
<br> <br>
  Other features include: Boot and Reset buttons and LED indicators for charging, charge standby, and USB connection. 

## List of Major Components/Datasheets

- [STM32U073KCU6](https://www.st.com/resource/en/datasheet/stm32u073c8.pdf) <br>

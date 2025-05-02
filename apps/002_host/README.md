# Host Controller Application (apps/002_host)

## Setup

The Host Controller application is implemented on a SAME54 Xplained Pro.  
The MPLAB&reg; X IDE Project can be found under [apps/002_host/firmware/WBZ451HPE.X](firmware/SAME54_XPRO.X)  
The Hex file is available under [hex/WBZ451HPE_PeripheralDevice.hex](../../hex/SAME54_XPRO_Host.hex)

The software has been created and tested with the following Software Development Tools:
- [MPLAB&reg; X IDE 6.25](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide)
- [MPLAB&reg; XC32 v4.60](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)
- [MPLAB&reg; Code Configurator v5.5.1](https://www.microchip.com/en-us/tools-resources/configure/mplab-code-configurator)
  - [csp v3.19.6](https://github.com/Microchip-MPLAB-Harmony/csp/tree/v3.19.6)
  - [core v3.13.5](https://github.com/Microchip-MPLAB-Harmony/core/tree/v3.13.5)
  - [CMSIS_5 5.9.0](https://github.com/Microchip-MPLAB-Harmony/wireless_ble/tree/v1.3.0)
  - [CMSIS-FreeRTOS v10.5.1](https://github.com/Microchip-MPLAB-Harmony/CMSIS-FreeRTOS/tree/v10.5.1)
- Packs:
  - [SAME54_DFP v3.8.234](https://packs.download.microchip.com/)

## Harmony Configuration
### Project Graph
<img src="../../images/apps_002_host/h3_project_graph.png"/>

### Pin Diagram and Settings
<img src="../../images/apps_002_host/h3_pin_diagram.png"/>  

<img src="../../images/apps_002_host/h3_pin_settings.png"/>

### USART Setting (for configuration and debug purpose)
<img src="../../images/apps_002_host//h3_sercom2_usart.png"/>

### CAN1 Setting (for CAN communication)
<img src="../../images/apps_002_host/h3_can1.png"/>

#### CAN1 - Bit Timing Calculation
<img src="../../images/apps_002_host/h3_can1_bit_timing_calculation.png"/>

#### CAN1 - FIFO Settings
<img src="../../images/apps_002_host/h3_can1_fifo.png"/>

#### CAN1 - Filter Settings
<img src="../../images/apps_002_host/h3_can1_filter.png"/>

[back](../../README.md/#software-setup)
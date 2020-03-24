# LOST60
Bluetooth Mechanical Keyboard Firmware

#### LOST60 PCB Version 2.0 (SPRING 2020)
<p align="center">
<img width="800px" src="https://raw.githubusercontent.com/coyt/LOST60/master/keyboardV2.jpg"/>
</p>

#### LOST60 PCB Version 1.0 (FALL 2019)
<p align="center">
<img width="800px" src="https://raw.githubusercontent.com/coyt/LOST60/master/keyboardpic.jpg"/>
</p>

*The Ultimate Hackable Bluetooth Mechanical Keyboard*

#### Overview

Codebase for the LOST60 Bluetooth 60% Mechanical Keyboard PCB. The PCB utilizes a Nordic Semiconductor [nrf52840 bluetooth SoC](https://www.nordicsemi.com/?sc_itemid=%7B2DC10BA5-A76E-40F8-836E-E2FC65803A71%7D) (Rigado BMD-340 module) as the sole processor.

Why custom firmare and not TMK/QMK/something else? Well, porting the necessary code is exceedingly difficult and time consuming due to the novelty of the nrf52840. Furthermore, some fundamental differences in featuresets exist - for example, the intent is to dynamically store and/or update configs and keymaps during runtime NOT during compile time as TMK/QMK do it currently. This should allow fast updates, dynamic config changes, and entirely new features. Streatch features include a hardware password manager built directly into the keyboard as well as support for HID hacking tools such as [Ducky Script](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript) 

#### Version 2.0 System Diagram

<p align="center">
<img width="800px" src="https://raw.githubusercontent.com/coyt/LOST60/master/BLEKeyboardV2SystemDiagram.jpg"/>
</p>

#### Hardware Features
* 60% Mechanical Keyboard design based on GH60 Layout
* USBC connector
* Bluetooth 5.0 support
* Full RGB underlighting with "neopixels"
* Independently addressable single color key backlingting
* Qi wireless charging
* Advanced battery charging and management circuitry
* Rotary encoder and TWO speakers for Synthesizer features (under development)
* Several expansion ports (QUIIC, GROVE, CUSTOM)

#### Software Features
* Bluetooth HID keyboard
* Simple Arduino style programming
* Bootloader allowing firmware updates over USB C (Program your keyboard WITH your keyboard!)

#### Planned Firmware Updates

- [ ] Fix BLE key modifier led's
- [ ] Organize Code
    - [x] Separate & orgainze core and header files
    - [ ] Develop architectural contorl schema - rtos tasks / state machine
    - [ ] Setup FreeRTOS inter-task communication
- [ ] USB HID / BLE HID autoswitching
- [ ] Low Power Modes
    - [ ] Power Consumption Testng
- [ ] Implement layers
- [ ] Implement unified config
    - [ ] FreeRTOS CLI setup
    - [ ] Configure persistent storage
    - [ ] Add JSON parser for QMK config generators

#### Planned Hardware Updates

- [x] Add coulomb counting IC / improve voltage and power measurement   (Complete in V2.0)
- [ ] Improve backlighting LED brightness 
- [x] Extend USB C connector out a bit                                  (Complete in V2.0)
- [x] Fix underlighting load switch fly wire                            (Complete in V2.0)
- [x] Fix LiPo charger ground pour problem & improve charge rate        (Complete in V2.0)
- [x] Remove onboard Lithium protection IC                              (Complete in V2.0)
- [ ] Add physical power switch connection point 
- [ ] Change wiring so I/O emulates Feather nrf52840 board
- [x] Add QSPI memory chip                                              (Complete in V2.0)

#### Development Setup

This code uses Adafruit's Arduino core for nRF52 which includes easy to use Bluetooth libraries that work on the nrf52840 SoC. To correctly run code, the Adafruit feather nrf52840 bootloader must be burned onto the nrf52840 SoC. This bootloader performs several functions including burning the Soft Device required for BLE and enabling USB com port and USB MSD programming. Burning it is easy to do with the Arduino IDE and a J-Link programmer. 

Simply install Arduino IDE and add the Adafruit Arduino core for nrf52 (board support package) - [follow their instructions here](https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/updating-the-bootloader). Then, connect a J-link to your nrf52840 via the SWD pins. Now, through the arduino IDE, select the burn bootlaoder option using J-Link. 

Afther the bootloader has been installed, you can uplaod code from PlatformIO via the J-link with the setings in this repo. 

#### Credits

* Core code based on ["Arduino Core for Adafruit Bluefruit nRF52 Boards"](https://github.com/adafruit/Adafruit_nRF52_Arduino)

* As per Adafruit's Github page, other libraries used are:

- adafruit-nrfutil is based on Nordic Semiconductor ASA's [pc-nrfutil](https://github.com/NordicSemiconductor/pc-nrfutil)
- [freeRTOS](https://www.freertos.org/) as operating system
- [tinyusb](https://github.com/hathach/tinyusb) as usb stack
- [nrfx](https://github.com/NordicSemiconductor/nrfx) for peripherals driver
- [littlefs](https://github.com/ARMmbed/littlefs) for internal file system
- [fatfs by elm-chan](http://elm-chan.org/fsw/ff/00index_e.html) for external file system
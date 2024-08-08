# LIFX Control Box
Basic physical control box for my LIFX smart light using potentiometers and an LCD display. 
I also utilized 3D models found on online for the housings, see below. 

<p align="center">
  <img src="https://raw.githubusercontent.com/AndyGutterman/LIFX_Control_Box/main/demo_images/overview.jpg" width="600" alt="Overview">
</p>

## Hardware

- **Potentiometers**: Adjust RGB values, brightness, and saturation.
- **ESP-32**: Connect pins and uses WiFi to interface with LIFX API.
- **LCD Display**: Show current values.

## Wiring

- **Potentiometer Pins**: Red `32`, Green `35`, Blue `34`, Brightness `33`, Saturation `36`
- **LCD I2C Address**: `0x27`

## Libraries

- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) - For controlling the LCD display.
- WIP

## Potentiometer Case (3D Printed)

<p align="center">
  <img src="https://raw.githubusercontent.com/AndyGutterman/LIFX_Control_Box/main/demo_images/pot_case.jpg" width="600" alt="Potentiometer Case">
</p>

3D Model source: [Volume Knob Controller for PC](https://www.printables.com/model/557980-volume-knob-controller-for-pc-deej)  
Modifications: Drilled 3 holes for output cables plus power and ground.

## LCD Case (3D Printed)

3D Model source: [LCD Case on Thingiverse](https://www.thingiverse.com/thing:3183550)


## Todo list / Future Improvements
  - Make LCD turn off automatically after a set period of time
  - Improve response time without hitting rate limits
  - Add Buttons for common presets
  - Add ability to change temperature
  - Create all-in-one case to contain all components (ESP32/Pots/Buttons/LCD)

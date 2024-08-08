# LIFX Control Box
- Basic physical control box for my LIFX smart light using potentiometers and an LCD display. 
- I also utilized 3D models found on online for the housings, see sources below to check them out.

<p align="center">
  <img src="https://raw.githubusercontent.com/AndyGutterman/LIFX_Control_Box/main/demo_images/overview.jpg" width="600" alt="Overview">
</p>

## Hardware

- **Potentiometers**: Adjust RGB values, brightness, and saturation.
- **ESP-32**: Connect pins and uses WiFi to interface with LIFX API.
- **LCD Display**: Show current values.

## Wiring

- **Potentiometer Pins**: Red `32`, Green `35`, Blue `34`, Brightness `33`, Saturation `36`
- **LCD Pins** SDA `21`, SCL`22`
- **LCD I2C Address** `0x27`

## Installation Instructions (Assuming identical connections)

1. **Clone me**
    ```bash
    git clone git@github.com:AndyGutterman/LIFX_Control_Box.git
    ```
    
2. **Navigate to the Project Directory**
    ```bash
    cd LIFX_esp_project/light_control/
    ```

3. **Install LiquidCrystal_I2C**
   Download, install from zip: [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) - For controlling the LCD display.
   
5. **Set Environment Variables**

    **(Windows)**
    ```bash
    set WIFI_SSID=your_wifi_ssid
    set WIFI_PASSWORD=your_wifi_password
    set LIFX_TOKEN=your_lifx_token
    set LIGHT_ID=your_light_id
    ```
6. **Run the Build Script**
    ```bash
    python build.py
    ```
    Verify that `config.h` is created or updated in your project directory. Otherwise, edit light_control.ino directly.

7. **Upload to ESP-32**
   


## Potentiometer Case (3D Printed)

<p align="center">
  <img src="https://raw.githubusercontent.com/AndyGutterman/LIFX_Control_Box/main/demo_images/pot_case.jpg" width="600" alt="Potentiometer Case">
</p>

3D Model source: [Volume Knob Controller for PC](https://www.printables.com/model/557980-volume-knob-controller-for-pc-deej)  
Modifications: Drilled 3 holes for output cables plus power and ground.

## LCD Case (3D Printed)

3D Model source: [LCD Case on Thingiverse](https://www.thingiverse.com/thing:3183550)


## Todo list / Future Improvements
  - Add Buttons for common presets
  - Add ability to change temperature
  - Create all-in-one case to contain all components (ESP32/Pots/Buttons/LCD)

import os

def get_input(prompt, default_value):
    user_input = input(prompt + f" (default: {default_value}): ")
    return user_input.strip() or default_value

def main():
    # Read environment variables with default values
    wifi_ssid = os.getenv('WIFI_SSID', 'default_ssid')
    wifi_password = os.getenv('WIFI_PASSWORD', 'default_password')
    lifx_token = os.getenv('LIFX_TOKEN', 'default_token')
    light_id = os.getenv('LIGHT_ID', 'default_light_id')

    # Check if any default values are used and prompt user if necessary
    if wifi_ssid == 'default_ssid' or wifi_password == 'default_password' or \
       lifx_token == 'default_token' or light_id == 'default_light_id':
        print("Some environment variables are not set. Please enter the values.")
        wifi_ssid = get_input('Enter your WiFi SSID', wifi_ssid)
        wifi_password = get_input('Enter your WiFi password', wifi_password)
        lifx_token = get_input('Enter your LIFX token', lifx_token)
        light_id = get_input('Enter your light ID', light_id)

    # Update config.h file with the configuration values
    config_content = f"""#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "{wifi_ssid}"
#define WIFI_PASSWORD "{wifi_password}"
#define LIFX_TOKEN "{lifx_token}"
#define LIGHT_ID "{light_id}"

#endif // CONFIG_H
"""

    with open('config.h', 'w') as config_file:
        config_file.write(config_content)
    
    print("config.h has been updated.")

if __name__ == '__main__':
    main()

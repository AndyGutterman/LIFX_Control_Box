#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const String lifx_token = LIFX_TOKEN;
const String light_id = LIGHT_ID;


// Pin definitions
const int potRedPin = 32;
const int potGreenPin = 35;
const int potBluePin = 34;
const int potBrightPin = 33;
const int potSaturationPin = 36;


// LCD setup   SDA:21   SCL:22
const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Previous values for comparison
int prevRed = -1, prevGreen = -1, prevBlue = -1;
float prevBrightness = -1.0, prevSaturation = -1.0;

// Timing for HTTP requests
unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 500; // 500 milliseconds

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

bool valueChanged(int newValue, int oldValue) {
  return abs(newValue - oldValue) > oldValue * 0.02; // 0.02 margin
}

bool valueChanged(float newValue, float oldValue) {
  return abs(newValue - oldValue) > oldValue * 0.02;  // 0.02 margin
}

void loop() {
  int potRedValue = analogRead(potRedPin);
  int potGreenValue = analogRead(potGreenPin);
  int potBlueValue = analogRead(potBluePin);
  int potBrightValue = analogRead(potBrightPin);
  int potSaturationValue = analogRead(potSaturationPin);

  int red = map(potRedValue, 0, 4095, 0, 255);
  int green = map(potGreenValue, 0, 4095, 0, 255);
  int blue = map(potBlueValue, 0, 4095, 0, 255);
  float brightness = map(potBrightValue, 0, 4095, 0, 1000) / 1000.0;
  float saturation = map(potSaturationValue, 0, 4095, 0, 1000) / 1000.0;



  bool updateLCD = false;

  if (valueChanged(red, prevRed) || valueChanged(green, prevGreen) || valueChanged(blue, prevBlue) ||
      valueChanged(brightness, prevBrightness) || valueChanged(saturation, prevSaturation)) {
    updateLCD = true;
    Serial.println("Value Changed.");
  }

  if (updateLCD) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RGB(" + String(red) + "," + String(green) + "," + String(blue) + ")");
    lcd.setCursor(0, 1);
    lcd.print("Br:");
    lcd.print(brightness, 2); // Display with 2 decimal places
    lcd.print(" Sat:");
    lcd.print(saturation, 2); // Display with 2 decimal places

    // Convert RGB to HSB
    float hue, sat, br;
    rgbToHsb(red, green, blue, hue, sat, br);  // Better color mixing compared to RGB values for some reason..

    // If all RGB values are 255, set saturation to 0 to make the light white (after updating the LCD)
    if (red == 255 && green == 255 && blue == 255) {
      saturation = 0;
    }

    updateLifxLight(hue, brightness, saturation);
  
    // Update prev values
    prevRed = red;
    prevGreen = green;
    prevBlue = blue;
    prevBrightness = brightness;
    prevSaturation = saturation;
  }

  delay(500);
}

// Convert RGB to Hue, Saturation, and Brightness
void rgbToHsb(int red, int green, int blue, float& hue, float& saturation, float& brightness) {
  float r = red / 255.0;
  float g = green / 255.0;
  float b = blue / 255.0;
  
  float maxVal = max(max(r, g), b);
  float minVal = min(min(r, g), b);
  float delta = maxVal - minVal;
  
  brightness = maxVal;
  
  if (delta == 0) {
    hue = 0; // Hue is irrelevant when saturation is 0
    saturation = 0;
  } else {
    saturation = delta / maxVal;
    
    if (maxVal == r) {
      hue = (g - b) / delta;
    } else if (maxVal == g) {
      hue = 2 + (b - r) / delta;
    } else {
      hue = 4 + (r - g) / delta;
    }
    
    hue *= 60;
    if (hue < 0) {
      hue += 360;
    }
  }
  
  saturation *= 100; // Convert to percentage
  brightness *= 100; // Convert to percentage
}

void updateLifxLight(float hue, float brightness, float saturation) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.lifx.com/v1/lights/id:" + light_id + "/state";

    http.begin(url);
    http.addHeader("Authorization", "Bearer " + lifx_token);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"color\":\"hue:" + String(hue) + " saturation:" + String(saturation) + "\", \"brightness\":" + String(brightness) + ", \"duration\": 1}";

    Serial.print("Payload: ");
    Serial.println(payload);

    int httpResponseCode = http.PUT(payload);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

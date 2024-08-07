#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "user";
const char* password = "pass";

// LIFX API token and light ID
const String lifx_token = "token";
const String light_id = "lightid";

// Pin definitions
const int potRedPin = 32;
const int potGreenPin = 35;
const int potBluePin = 34;
const int potBrightPin = 33;
const int potSaturationPin = 36;
// const int buttonBlackPin = 17;
const int buttonBluePin = 16;
const int buttonYellowPin = 4;

// LCD configuration
const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Previous values for comparison
int prevRed = -1, prevGreen = -1, prevBlue = -1;
float prevBrightness = -1.0;
float prevSaturation = -1.0;

// Button state tracking
// bool blackButtonPressed = false;
bool blueButtonPressed = false;
bool yellowButtonPressed = false;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  lcd.setCursor(0, 1);
  lcd.print("Please wait...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Clear LCD and show the connection status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());
  
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(buttonYellowPin, INPUT_PULLUP);
  // pinMode(buttonBlackPin, INPUT_PULLUP);
}

bool valueChanged(int newValue, int oldValue) {
  if (oldValue == -1) return true;
  return abs(newValue - oldValue) > oldValue * 0.02;
}

bool valueChanged(float newValue, float oldValue) {
  if (oldValue == -1.0) return true;
  return abs(newValue - oldValue) > oldValue * 0.02;
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

  // bool buttonBlackState = digitalRead(buttonBlackPin) == LOW;
  bool buttonBlueState = digitalRead(buttonBluePin) == LOW;
  bool buttonYellowState = digitalRead(buttonYellowPin) == LOW;

  // Button actions
  //  if (buttonBlackState) {
  //   blackButtonPressed = true;
  //   blueButtonPressed = false;
  //   yellowButtonPressed = false;
  //   updateLifxLightOff();
  // } else 
  if (buttonBlueState) {
    // blackButtonPressed = false;
    blueButtonPressed = true;
    yellowButtonPressed = false;
    updateLifxLightKelvin(5000); // Cool temperature
  } else if (buttonYellowState) {
    // blackButtonPressed = false;
    blueButtonPressed = false;
    yellowButtonPressed = true;
    updateLifxLightKelvin(2700); // Warm temperature
  } else {
    // blackButtonPressed = false;
    blueButtonPressed = false;
    yellowButtonPressed = false;
    
    // Update light settings if no button is pressed
    bool updateLCD = false;

    if (valueChanged(red, prevRed) || valueChanged(green, prevGreen) || valueChanged(blue, prevBlue) ||
        valueChanged(brightness, prevBrightness) || valueChanged(saturation, prevSaturation)) {
      updateLCD = true;
      Serial.println("Value Changed.");
      
      updateLifxLight(red, green, blue, brightness, saturation);
    }

    if (updateLCD) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("R:");
      lcd.print(red);
      lcd.print(" G:");
      lcd.print(green);
      lcd.print(" B:");
      lcd.print(blue);
      
      lcd.setCursor(0, 1);
      lcd.print("Br:");
      lcd.print(brightness, 2); // Display with 2 decimal places
      lcd.print(" Sat:");
      lcd.print(saturation, 2); // Display with 2 decimal places

      // Update prev values
      prevRed = red;
      prevGreen = green;
      prevBlue = blue;
      prevBrightness = brightness;
      prevSaturation = saturation;
    }
  }

  // Display button state on LCD
  // lcd.setCursor(8, 1);
  // lcd.print(blackButtonPressed ? "0" : " ");
  // lcd.setCursor(9, 1);
  // lcd.print(blueButtonPressed ? "1" : " ");
  // lcd.setCursor(10, 1);
  // lcd.print(yellowButtonPressed ? "2" : " ");

  delay(500);  // Delay to avoid flooding the monitor
}

void updateLifxLight(int red, int green, int blue, float brightness, float saturation) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.lifx.com/v1/lights/id:" + light_id + "/state";

    http.begin(url);
    http.addHeader("Authorization", "Bearer " + lifx_token);
    http.addHeader("Content-Type", "application/json");

    // Format RGB and other settings
    String payload = "{\"color\":\"rgb:" + String(red) + "," + String(green) + "," + String(blue) + "\", \"brightness\":" + String(brightness, 2) + ", \"duration\": 1}";

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

void updateLifxLightOff() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.lifx.com/v1/lights/id:" + light_id + "/state";

    http.begin(url);
    http.addHeader("Authorization", "Bearer " + lifx_token);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"power\":\"off\"}";

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

void updateLifxLightKelvin(int kelvin) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.lifx.com/v1/lights/id:" + light_id + "/state";

    http.begin(url);
    http.addHeader("Authorization", "Bearer " + lifx_token);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"color\":\"kelvin:" + String(kelvin) + "\", \"brightness\":1.0, \"duration\": 1}";

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

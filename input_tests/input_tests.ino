#include <LiquidCrystal_I2C.h>

const int potRedPin = 32;
const int potGreenPin = 35;
const int potBluePin = 34;
const int potBrightPin = 33;
const int potSaturationPin = 36;
// const int buttonBlackPin = 17;
const int buttonBluePin = 16;
const int buttonYellowPin = 4;


const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int prevRed = -1, prevGreen = -1, prevBlue = -1;    
float prevBrightness = -1.0;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(buttonYellowPin, INPUT_PULLUP);
  pinMode(buttonBlackPin, INPUT_PULLUP);
}

bool valueChanged(int newValue, int oldValue) {
  return abs(newValue - oldValue) > oldValue * 0.02;
}

bool valueChanged(float newValue, float oldValue) {
  return abs(newValue - oldValue) > oldValue * 0.02;
}

void loop() {
  int potRedValue = analogRead(potRedPin);
  int potGreenValue = analogRead(potGreenPin);
  int potBlueValue = analogRead(potBluePin);
  int potBrightValue = analogRead(potBrightPin);

  int red = map(potRedValue, 0, 4095, 0, 255);
  int green = map(potGreenValue, 0, 4095, 0, 255);
  int blue = map(potBlueValue, 0, 4095, 0, 255);
  float brightness = map(potBrightValue, 0, 4095, 0, 1000) / 1000.0;
  
  bool buttonBlackState = digitalRead(buttonBlackPin) == LOW;
  bool buttonBlueState = digitalRead(buttonBluePin) == LOW;
  bool buttonYellowState = digitalRead(buttonYellowPin) == LOW;

  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" Green: ");
  Serial.print(green);
  Serial.print(" Blue: ");
  Serial.print(blue);
  Serial.print(" Brightness: ");
  Serial.print(brightness);
  Serial.print(" Blue Button: ");
  Serial.print(buttonBlueState ? "Pressed" : "Released");
  Serial.print(" Black Button: ");
  Serial.print(buttonBlackState ? "Pressed" : "Released");
  Serial.print(" Yellow Button: ");
  Serial.println(buttonYellowState ? "Pressed" : "Released");

  // Serial print raw analog values
  Serial.print("Raw Red: ");
  Serial.println(potRedValue);
  Serial.print("Raw Green: ");
  Serial.println(potGreenValue);
  Serial.print("Raw Blue: ");
  Serial.println(potBlueValue);
  Serial.print("Raw Brightness: ");
  Serial.println(potBrightValue);

  bool updateLCD = false;

  if (valueChanged(red, prevRed) || valueChanged(green, prevGreen) || valueChanged(blue, prevBlue) ||
      valueChanged(brightness, prevBrightness)) {
    updateLCD = true;
    Serial.println("Value Changed.");
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

    if (buttonBlackState) {
      lcd.setCursor(8, 1);
      lcd.print("0");
    }
    if (buttonBlueState) {
      lcd.setCursor(9, 1);
      lcd.print("1");
    }
    if (buttonYellowState) {
      lcd.setCursor(10, 1);
      lcd.print("2");
    }

    // Update prev values
    prevRed = red;
    prevGreen = green;
    prevBlue = blue;
    prevBrightness = brightness;
  }

  delay(500);  // Delay to avoid flood monitor
}

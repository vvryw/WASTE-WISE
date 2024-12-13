#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int result = -1;

void setup() {
  Serial.begin(9600);

  // LCD initialization
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Proj: Bin Opener!");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Powered by CRU!");
  delay(2000);
  lcd.clear();

  // Servo initialization
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo2.write(10);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  
  for (int i = 90; i > 0; i--) {
    servo1.write(i);
    servo2.write(i);
    servo3.write(i);
    delay(30);
    lcd.setCursor(0, 1);
    lcd.print("Test: ");
    lcd.print(i);
    delay(30);
  }
  lcd.clear();
  for (int i = 0; i < 90; i++) {
    servo1.write(i);
    servo2.write(i);
    servo3.write(i);
    delay(30);
    lcd.setCursor(0, 1);
    lcd.print("Test: ");
    lcd.print(i);
    delay(30);
  }
  delay(3000);
  lcd.clear();

    servo1.write(90);
    servo2.write(90);
    servo3.write(90);

  Serial.setTimeout(2000);
}

void loop() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  lcd.setCursor(0, 0);
  lcd.print("Reading: ");
  if (Serial.available()) {
    // int value = Serial.parseInt();
    String myString = Serial.readStringUntil('\n');
    //Serial.println(myString);
    if (myString.length() > 0) {
      Satring firstChr = String(myString.charAt(0));
      int result = firstChar.toInt();
      lcd.setCursor(9, 0);      
      lcd.print(result);
      lcd.setCursor(0, 1);
      lcd.print("Opening...: ");

      if (result == 0) { 
        lcd.setCursor(0,1);
        lcd.print("PlasticBottle");               
        for (int i = 90; i > 0; i--) {
          servo1.write(i);
          delay(30);
        }
        delay(2000);
        for (int i = 0; i < 90; i++) {
          servo1.write(i);
          delay(30);
        }
      } else if (result == 1) {
        lcd.setCursor(0,1);
        lcd.print("Tin Can");
        for (int i = 90; i > 0; i--) {
          servo2.write(i);
          delay(30);
        }
        delay(2000);
        for (int i = 0; i < 90; i++) {
          servo2.write(i);
          delay(30);
        }
      } else if (result == 2) {
        lcd.setCursor(0,1);
        lcd.print("Paper");
        for (int i = 90; i > 0; i--) {
          servo3.write(i);
          delay(30);
        }
        delay(2000);
        for (int i = 0; i < 90; i++) {
          servo3.write(i);
          delay(30);
        }        
      }
      lcd.clear();
    }
  }
}
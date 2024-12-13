#include <Servo.h>
#include <Ultrasonic.h>
Servo myServo;
String mycmd;
#define BUZZER 9
const int pingPin = 6;
int inPin = 7;
int button = 8 ;
int FuncON = 0;
int val = 0;
int pushed = 0 ;
int a = 0;
int cm ;
int cm2;
int i=0;
int poten;
int potpin = 1;
int van=1;
const int trigPin = 11;
int echoPin = 12;
boolean stop = false;
boolean dop = false;
int lightpin = A0;
int sensorValue = 0; // variable to store the value coming from the 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); //0x27 

void setup() {
    // need to useserial monitor ==> using Serial.begin(9600);
    Serial.begin(9600);
    //ไม่เกี่ยว
    pinMode(button, INPUT);
    pinMode(BUZZER,OUTPUT);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    myServo.attach(10);
    lcd.begin();
    lcd.backlight();
}
void soundAlarm(){
    digitalWrite(BUZZER,HIGH);
    tone(BUZZER,500);
    delay(10);
    noTone(BUZZER);
    delay(50);
}
void dclose(){
  if(stop == true){
      long duration, cm;
      pinMode(trigPin, OUTPUT);
      pinMode(3, OUTPUT);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(trigPin, LOW);
      pinMode(echoPin, INPUT);
      duration = pulseIn(echoPin, HIGH);
  
      cm = microsecondsToCentimeters(duration);
      Serial.print("Dclosed : ");
      Serial.println(cm);
      if(cm<4){
        if(sensorValue < 300){
          lcd.setCursor(0,0);
          lcd.print("GATE CLOSING");
          myServo.write(105);
          delay(500);
          lcd.clear();
          myServo.write(90);
          stop = false;
          dop = false;
        }
        else{
          Serial.print(cm);
          lcd.setCursor(0,0);
          lcd.print("GATE CLOSING");
          myServo.write(105);
          delay(800);
          lcd.clear();
          myServo.write(90);
          stop = false;
          dop = false;
        }
      }
    }
}
void pot() {
  poten = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  poten = map(poten, 0, 1023, 80, 100);     // scale it for use with the servo (value between 0 and 180)
  myServo.write(poten);                  // sets the servo position according to the scaled value
  if(poten > 93){
    dop=false; 
    lcd.setCursor(0,0);
    lcd.print("GATE CLOSING");
  }
  if(poten < 87){
    dop=true; 
    lcd.setCursor(0,0);
    lcd.print("GATE OPENING");
  }
  if(poten <= 87 and poten >=93){
    lcd.clear();
  }
  delay(15);                           // waits for the servo to get there
}

void sensorflood(){

    long duration, cm2;
    pinMode(pingPin, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    pinMode(inPin, INPUT);
    duration = pulseIn(inPin, HIGH);
    cm2 = microsecondsToCentimeters(duration);
    Serial.print("Flood : ");
    Serial.println(cm2);
    if(cm2 <= 6 ){
       delay(10);
       soundAlarm();
       light();
       stop = false;
       lcd.setCursor(0,0);
       lcd.print("FLOOD !!");
       if(van==1){
        if(dop==false){
          myServo.write(75);
          delay(800);
          myServo.write(90);
        }
       }
        van++;
      }
    if(cm2>6){
      van=1;
      if(dop == true)stop = true;
      delay(10);
      lcd.clear();
     }
  
}

void light(){
  digitalWrite(5, HIGH);
  delay(10);
  digitalWrite(5, LOW);
  delay(10);
}
void lighthold(){
  sensorValue = analogRead(lightpin);
  Serial.print("LDR : ");
  Serial.println(sensorValue);
  delay(100);
  if(sensorValue <300){
    digitalWrite(13,1);
    lcd.setCursor(0,1);
    lcd.print("Night Time");
  }
  else{
    digitalWrite(13,0);
    lcd.clear();
  }
}


void loop(){

    // ทำไปเรื่อยๆ ต่อให่อันอื่นทำ ไม่พัง making multi-function
    while(Serial.available()==0){
      pot();
      sensorflood();
      dclose();
      lighthold();
    }

    // String mycmd;
    // python sent data; Arduino read the data from Serial monitor until '\r'
    mycmd = Serial.readStringUntil('\r');
    myServo.write(90);
    // check data from python which mycmd is one of data in arduino
    if(mycmd=="AON" or "GRAM" or "JO"){
      if(sensorValue < 300){
        lcd.setCursor(0,0);
        lcd.print("GATE OPENING");
        myServo.write(75);
        delay(500);
        myServo.write(90);
        lcd.clear();
        stop = true;
        dop = true;
        }
      else{
        lcd.setCursor(0,0);
        lcd.print("GATE OPENING");
        myServo.write(75);
        delay(800);
        lcd.clear();
        myServo.write(90);
        stop = true;
        dop = true;
        }
      }
    }



long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}
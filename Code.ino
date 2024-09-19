#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD's I2C address

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 4;
const int ledgreenPin = 2;
const int buzzerPin = 5;
const int pump = 8;

long duration;
int distance;
int percentage;

int maxDistance = 17; // 17 cm represents 0%
int minDistance = 4;  // 4 cm represents 100%

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.clear();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= minDistance) 
  {
    percentage = 100;
    pump_on_off(false);
    digitalWrite(pump, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledgreenPin, HIGH);
    noTone(buzzerPin);
  } 
  else if (distance >= maxDistance) 
  {
    percentage = 0;
    pump_on_off(true);
    digitalWrite(pump, HIGH);
    digitalWrite(ledgreenPin, LOW);
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);
  } 
  else {
    int distanceRange = maxDistance - minDistance;
    int distanceFromMin = distance - minDistance;
    percentage = 100 - (distanceFromMin * 100 / distanceRange / 10) * 10;
    percentage = constrain(percentage, 0, 100);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledgreenPin, LOW);
    noTone(buzzerPin);
  }

  lcd.setCursor(0, 1);
  lcd.print(percentage);
  lcd.print("% ");

  delay(500);
}

void pump_on_off(bool pumpState) 
{
  lcd.setCursor(0, 0);
  if (pumpState) {
    lcd.print("Water Pump On");
  } else {
    lcd.print("Water Pump Off");
  }
}
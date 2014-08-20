/*
 HC-SR04 Ping distance sensor]
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 13 
 Trig to Arduino pin 12
 
 LED pos to Arduino pin 10 (ok distance indicator)
 LED pos to Arduino pin 11 (too close distance indicator)
 330 ohm resistor (or something close) to both LED neg and GRD power rail
 
 */

#include <LiquidCrystal.h>  // LCD library

const int trigPin = 13;
const int echoPin = 8;

const int outOfBoundsHIGH = 60;
const int outOfBoundsLOW = 0;

LiquidCrystal lcd(12,11,5,4,3,2);

void setup() {
  Serial.begin (9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("I am Ping Man.");
  delay(5000);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration, distance0, distance;

  //trigger a PING
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  //listen for the echo & calculate the distance   
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1 / 2.54;
  if (distance > outOfBoundsHIGH) distance = outOfBoundsHIGH;
  if (distance < outOfBoundsLOW)  distance = outOfBoundsLOW;

  lcd.setCursor(0,0);
  lcd.print("                   ");
  lcd.setCursor(0,0);
  lcd.print("Ping: ");
  lcd.print(distance);
  lcd.print(" inches");
  Serial.print(distance);
  Serial.println(" inches");

  delay(500);
}



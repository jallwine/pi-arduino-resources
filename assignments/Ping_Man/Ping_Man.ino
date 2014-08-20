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

const int tooCloseLED = 9;
const int seriouslyTooCloseLED = 10;
const int panicStrickenLED = 6;
const int YOkillinMeLED = 7;

const int tooClose = 26;
const int seriouslyTooClose = 20;
const int panicStricken = 12;
const int YOkillinMe = 6;

const int outOfBoundsHIGH = 60;
const int outOfBoundsLOW = 0;

const float newWeight = 1.0;

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
  pinMode(tooCloseLED, OUTPUT);
  pinMode(seriouslyTooCloseLED, OUTPUT);
  pinMode(panicStrickenLED, OUTPUT);
  pinMode(YOkillinMeLED, OUTPUT);
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
  distance0 = (duration/2) / 29.1 / 2.54;
  if (distance0 > outOfBoundsHIGH) distance0 = outOfBoundsHIGH;
  if (distance0 < outOfBoundsLOW)  distance0 = outOfBoundsLOW;

  distance = runningAverage(distance0, distance);	// <10>
  lcd.setCursor(0,0);
  lcd.print("                   ");
  lcd.setCursor(0,0);
  lcd.print("Ping: ");
  lcd.print(distance);
  lcd.print(" inches");

  // light increasingly more LED distance warning ligt if distance is tooClose or less
  if (distance >= outOfBoundsHIGH) 
  { 
    Serial.println("Out of Bounds");
     lcd.setCursor(0,1);
     lcd.print("... or more    ");
    }
  else if (distance > tooClose && distance < outOfBoundsHIGH) 
  {
    digitalWrite(YOkillinMeLED,LOW);
    digitalWrite(panicStrickenLED,LOW);
    digitalWrite(seriouslyTooCloseLED,LOW);     
    digitalWrite(tooCloseLED,LOW);
    Serial.print(distance);
    Serial.println(" inches");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("No problem!");  }
  else if (distance <= tooClose && distance > seriouslyTooClose)
  {
    digitalWrite(YOkillinMeLED,LOW);
    digitalWrite(panicStrickenLED,LOW);
    digitalWrite(seriouslyTooCloseLED,LOW);     
    digitalWrite(tooCloseLED,HIGH);
    Serial.print(distance);
    Serial.println(" inches is a little bit close");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("A bit close");
  }
  else if (distance <= seriouslyTooClose && distance > panicStricken)
  {
    digitalWrite(YOkillinMeLED,LOW);
    digitalWrite(panicStrickenLED,LOW);
    digitalWrite(seriouslyTooCloseLED,HIGH);
    digitalWrite(tooCloseLED,HIGH);
    Serial.print(distance);
    Serial.println(" inches ... dude, this is seriously too close");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Too close dude");  }
  else if (distance <= panicStricken && distance > YOkillinMe)
  {
    digitalWrite(YOkillinMeLED,LOW);
    digitalWrite(panicStrickenLED,HIGH);
    digitalWrite(seriouslyTooCloseLED,HIGH);     
    digitalWrite(tooCloseLED,HIGH);
    Serial.print(distance);
    Serial.println(" inches ... way too close, I'm panic stricken");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Seriously close");  }
  else
  {
    digitalWrite(YOkillinMeLED,HIGH);
    digitalWrite(panicStrickenLED,HIGH);
    digitalWrite(seriouslyTooCloseLED,HIGH);     
    digitalWrite(tooCloseLED,HIGH);
    Serial.print(distance);
    Serial.println(" inches ... way waY WAY too close, YO killin me");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("You're killin me!");  }

  delay(500);
}

int runningAverage(int input, int old) {
  return newWeight*input + (1-newWeight)*old;	// <14>
}



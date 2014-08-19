// RGB Color Analyzer

const int gsr1Pin = 7;
const int gsr0Pin = 6;
const int gsg1Pin = 5;
const int gsg0Pin = 4;
const int gsb1Pin = 3;
const int gsb0Pin = 2;

const int ledPin = 8;

const int redInput = A0;
const int greenInput = A1;
const int blueInput = A2;

const int redOutput = 9;
const int greenOutput = 10;
const int blueOutput = 11;

int red = -1;
int green = -1;
int blue = -1;
int ered = -1;
int eblue= -1;
int egreen = -1;
int redValue = -1;
int greenValue = -1;
int blueValue = -1;
int test = -1;

const float newWeight = 0.7;	// <6>

const boolean debug = false;

void setup() {
  Serial.begin(9600);  
  pinMode(gsr1Pin, OUTPUT); 
  pinMode(gsr0Pin, OUTPUT);
  pinMode(gsg1Pin, OUTPUT);
  pinMode(gsg0Pin, OUTPUT);
  pinMode(gsb1Pin, OUTPUT);
  pinMode(gsb0Pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(redOutput, OUTPUT);
  pinMode(greenOutput, OUTPUT);
  pinMode(blueOutput, OUTPUT); 

  digitalWrite(ledPin, HIGH);

  digitalWrite(gsr1Pin, LOW); 
  digitalWrite(gsr0Pin, LOW);
  digitalWrite(gsg1Pin, LOW);
  digitalWrite(gsg0Pin, LOW);
  digitalWrite(gsb1Pin, LOW);
  digitalWrite(gsb0Pin, LOW);
}

void loop() {
  int redIn = analogRead(redInput);
  int greenIn = analogRead(greenInput);
  int blueIn = analogRead(blueInput);

  redValue = redIn * 10 / 1.0;
  greenValue = greenIn * 10 / 0.75;//0.7;
  blueValue = blueIn * 10 / 0.55;//0.8;

  redValue = map(redValue, 0, 1023, 0, 255);
  greenValue = map(greenValue, 0, 1023, 0, 255);
  blueValue = map(blueValue, 0, 1023, 0, 255); 

  if(redValue > 255) redValue = 255;
  if(greenValue > 255) greenValue = 255;
  if(blueValue > 255) blueValue = 255;  

  red = runningAverage(redValue, red);
  green = runningAverage(greenValue, green);
  blue = runningAverage(blueValue, blue);

  if(red < 200 || green < 180 || blue < 180) {
  green = green - red * 0.3;
  blue = blue - red * 0.3;  
  }

  ered = easing(red);
  egreen = easing(green);
  eblue = easing(blue);

  if(debug == false) {
    Serial.print("RGB = ( "); 
    Serial.print(ered); 
    Serial.print(" ");
    Serial.print("\t "); 
    Serial.print(egreen); 
    Serial.print(" ");
    Serial.print("\t "); 
    Serial.print(eblue); 
    Serial.println(" )");
  } 
  else {
    Serial.print("RGB = ( "); 
    Serial.print(redIn); 
    Serial.print(" "); 
    Serial.print(red); 
    Serial.print(" "); 
    Serial.print(ered); 
    Serial.print(" ");
    Serial.print("\t "); 
    Serial.print(greenIn); 
    Serial.print(" "); 
    Serial.print(green); 
    Serial.print(" "); 
    Serial.print(egreen); 
    Serial.print(" ");
    Serial.print("\t "); 
    Serial.print(blueIn); 
    Serial.print(" "); 
    Serial.print(blue); 
    Serial.print(" "); 
    Serial.print(eblue); 
    Serial.println(" )");
  }
  setColor(ered,egreen,eblue);
  //setColor(1,255,1);
  delay(100);  
}

int runningAverage(int input, int old) {
  return newWeight*input + (1-newWeight)*old;
}

int easing(int input) {
  float percent = input / 255.0f;
  return 255.0f * percent * percent * percent * percent;
}

int setColor(int r, int g, int b) {	// <16>
  analogWrite(redOutput, r);	// <17>
  analogWrite(greenOutput, g);
  analogWrite(blueOutput, b);  
}



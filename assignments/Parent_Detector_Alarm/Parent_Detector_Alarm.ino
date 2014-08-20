// Parent Detector - with alarm

const int sensorPin = 8;  // on the detector
const int ledPin = 13;    // blue LED on Arduino
int speakerPin = 10;      // tone generator
const int learningPeriod = 30*1000; // 30 seconds for learning period.
int learningCompleted = 0;

void setup() {
  Serial.begin(115200);
  pinMode(speakerPin, OUTPUT);
  pinMode(sensorPin, INPUT); 
  Serial.println("Start learning for next 30 seconds.");
  pinMode(ledPin, OUTPUT);
}

// create an alarm 
void alarm() 
{
  tone(speakerPin, 300, 40);
  delay(100);
  tone(speakerPin, 440, 20);
  delay(100);
  tone(speakerPin, 540, 40);
  delay(100);
}

// main loop
void loop() {
   if(millis() < learningPeriod) {
     return; // Sensor has not yet learned its environment.  
   }
   if(learningCompleted == 0) {
     learningCompleted = 1;
     Serial.println("Learning completed.");
   }
   if(digitalRead(sensorPin) == HIGH) {
     Serial.println("Movement detected");
     alarm();
     digitalWrite(ledPin, HIGH);
   } else {
     Serial.println("No movement detected");
     digitalWrite(ledPin, LOW);     
   }
   delay(100);
}

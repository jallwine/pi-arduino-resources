// The Perent Detector

const int sensorPin = 8;
const int ledPin = 13;
const int learningPeriod = 30*1000; // time in ms
int learningCompleted = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT); 
  Serial.println("Starting to learn unmoving environment...");	// <2>
  pinMode(ledPin, OUTPUT);
}

void loop() {
   if(millis() < learningPeriod) {	// millis = time since last boot
     delay(20); // we need the delay ... otherwise will consume 100% cpu
     return;	// 'return' finsihes the 'loop' subroutine
   }
   if(learningCompleted == 0) { // the sensor learns only once
     learningCompleted = 1;
     Serial.println("Learning completed.");
   }

   if(digitalRead(sensorPin) == HIGH) {	// sensor pin goes HIGH when movemenyt is detected
     Serial.println("Movement detected");
     digitalWrite(ledPin,HIGH);
   } else {
     Serial.println("No movement detected");
     digitalWrite(ledPin,LOW);     
   }
   delay(100);
}

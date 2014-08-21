/*
Children's Museum of Bozeman STEAMlab
Robotic Arm Sketch #1
"What Should I Do With The Empty Roll?"

Use a robotic arm to move an empty toilet paper roll from one side of
an exhibit to the other ... perpetually.

 Sweep a servo back and forth through its full range of motion.

Adapted from SparkFun SIK

Servos:
 A "servo", short for servomotor, is a motor that includes 
 feedback circuitry that allows it to be commanded to move to
 specific positions. The two servos used here are on the small side. Larger servos
 are used extensively in robotics to control mechanical arms,
 hands, etc. You could use a small servo to make a (tiny) robot arm,
 aircraft control surface, or anywhere something needs to be
 moved to specific positions.
 
Hardware connections: 
 The servo has a cable attached to it with three wires.
 Because the cable ends in a socket, you could use jumper wires
 to connect between the Arduino and any one servo. In this case, since
 There are more than one servo, it's best to use a breadboard to 
 interface with the Arduino.
 
 Connect the RED wire (power) to 5 Volts (5V)
 Connect the WHITE wire (signal) to digital pin 9
 Connect the BLACK wire (ground) to ground (GND)
 
 Note that servos can use a lot of power, which can cause your
 Arduino to reset or behave erratically. If you're using large
 servos or many of them, it's best to provide them with their
 own separate 5V supply. See this Arduino Forum thread for info:
 http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1239464763
 
 This sketch adapted from one written by SparkFun Electronics,
 with lots of help from the Arduino community.
 This code is completely free for any use.
 Visit http://learn.sparkfun.com/products/2 for SIK information.
 Visit http://www.arduino.cc to learn about the Arduino.
 
 */


// If we had to write a sketch to control a servo from scratch,
// it would be a lot of work. Fortunately, others have done the
// hard work for you. We're going to include a "library"
// that has the functions needed to drive servos.

// A library is an set of additional functions you can add to
// your sketch. Numerous libraries are available for many uses,
// see http://arduino.cc/en/Reference/Libraries for information
// on the standard libraries, and Google for others. When you're
// using a new part, chances are someone has written a library
// for it.

#include <Servo.h>  // servo library

// Once you "include" a library, you'll have access to those
// functions. You can find a list of the functions in the servo
// library at: http://arduino.cc/en/Reference/Servo
// Most libraries also have example sketches you can load from
// the "file/examples" menu.
// 
// In this sketch, we will use the XXX.write(POS) function.  This
// function allows us to move servo XXX to position POS, where POS 
// is a number from 0 to `180.  Most servos can move 180 degrees.  
// Some servos can move 360 degrees.


// Now we'll create a servo "object", called myservo. You should
// create one of these for each servo you want to control. 
// You can control a maximum of twelve servos on the Uno 
// using this library. (Other servo libraries may let you
// control more). Note that this library disables PWM on
// pins 9 and 10!

Servo armServo;  // servo control object for the robotic arm
Servo handServo;  // servo control object for the robotic hand

int delaySpeed = 1000;  // delay between servo actions 
int openHandPos = 160;  // the position, in degrees, of the servo position of an open robot hand
int closeHandPos = 10;  // the position, in degrees, of the servo position of a closed robot hand
int armSpeed = 2;       // the speed with which the arm moves to deposit the toilet paper roll (1 = slowest, 2 = faster, etc.)

void setup()
{
  // We'll write some notes to the serial monitor as we go
  Serial.begin(9600);

  // We'll now "attach" the armServo object to digital pin 9
  // and the handServo object to digital pin 10.
  // If you want to control more than one servo, attach more
  // servo objects to the desired pins (must be digital).

  // Attach tells the Arduino to begin sending control signals
  // to the servo. Servos require a continuous stream of control
  // signals, even if you're not currently moving them.
  // While the servo is being controlled, it will hold its 
  // current position with some force. If you ever want to
  // release the servo (allowing it to be turned by hand),
  // you can call servo1.detach().

  armServo.attach(9);  // the arm uses pin 9
  handServo.attach(10); // the hand uses pin 10
  // the servos will initialize at 90 degrees, once attached.  
  // this is home position, and where the arm begins.

  // open the hand to begin our process
  openHand(openHandPos);  
}


void loop()
{
  Serial.println("Begin loop");

  // the object begins at 0 degrees
  // move to the object, pick it up, bring it to th eother side & drop it
  slowMoveArm(90, 5, armSpeed);         // move to the object
  closeHand(closeHandPos);              // pick it up
  slowMoveArm(5, 180, armSpeed);        // bring it to the other side
  openHand(openHandPos);                // drop it
  armServo.write(90);                   // return to home position
  delay(delaySpeed);

  // now the object is at 180 degrees
  slowMoveArm(90, 180, armSpeed);        // move to the object
  closeHand(closeHandPos);               // pick it up
  slowMoveArm(180, 5, armSpeed);         // bring it to th eoriginal side
  openHand(openHandPos);                 // drop it
  armServo.write(90);                    // return to home position
  delay(delaySpeed);

  Serial.println("END of loop");
}


// this function slowly moves the arm from one position to another
// hardcoded to the 'armServo' object
void slowMoveArm(int thisPos, int nextPos, int armSpeed)
{
  Serial.print("slowMoveArm( ");
  Serial.print(thisPos);
  Serial.print(",");
  Serial.print(nextPos);
  Serial.println(" )");

  if(thisPos < nextPos) 
  {
    for(int x = thisPos; x <= nextPos; x=x+armSpeed)
    {
      armServo.write(x);  // move to next position
      delay(20);          // short pause to allow it to move
    }
  } 
  else 
  {
    for(int x = thisPos; x >= nextPos; x=x-armSpeed)
    {
      armServo.write(x);  // move to next position
      delay(20);          // short pause to allow it to move
    }
  }
  delay(delaySpeed);
}

//function to open the hand
void openHand(int pos)
{
  Serial.print("openHand (");
  Serial.print(pos);
  Serial.println(")");
  handServo.write(pos);
  delay(delaySpeed);
}

//function to close the hand
void closeHand(int pos)
{
  Serial.print("closeHand (");
  Serial.print(pos);
  Serial.println(")");
  handServo.write(pos);
  delay(delaySpeed);
}







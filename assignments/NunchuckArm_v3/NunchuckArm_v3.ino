/*
 * NunchuckArm
 *
 *
 * Derived from code by: 
 * 2007 Tod E. Kurt, http://todbot.com/blog/
 * The Wii Nunchuck reading code is taken from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */

#include <Wire.h>   // I2C library
#include <Servo.h>  // servo library

Servo armServo;  // servo control object for the robotic arm
Servo handServo;  // servo control object for the robotic hand

// initialize the 6 values we want from the nunchuck with middle values
int joyX = 90;
int joyY = 90;
int accX = 90;
int accY = 90;
int zButton = 0;
int cButton = 0;

// initialize the smoothed nunchuck values
int joyXs = 90;
int joyYs = 90;
int accXs = 90;
int accYs = 90;

// set the default parameter to control nunchuck sensitivity to inputs
// normal humans = (0.1 - 0.2), fighter pilots = (0.2 - 0.3), hard to control = (0.3 on up)
// try 0.2 for the joystick & 0.08 from the accelerometer
const float wgtAvgSpeed = 0.2;
int useJoystick = 1;      // default parameter for using joyStick (1) or Accelerometer (0)

int debugNumchuck = 1;    // prints more data about numchuck position
int debugArmMovement = 1; // excersize robotic arm on initialization 

// set some robotic arm movement constants
const int delaySpeed = 20;  // delay between servo actions 
const int openHandPos = 160;  // the position, in degrees, of the servo position of an open robot hand
const int closeHandPos = 10;  // the position, in degrees, of the servo position of a closed robot hand
const int armSpeed = 1;       // the speed with which the arm moves to deposit the toilet paper roll (1 = slowest, 2 = faster, etc.)

void setup()
{
  Serial.begin(19200);

  // initialize nunchuck
  // nunchuck uses analog pins 2 & 3 as (fake-out) gnd & pwr,
  // nunchuck uses analog pins 3 & 4 as clk & data 
  nunchuck_setpowerpins();
  nunchuck_init(); // send the initilization handshake

  // initialize servos
  armServo.attach(9);  // the arm uses pin 9
  handServo.attach(10); // the hand uses pin 10

    // excercise the robot to initialize
  openHand(openHandPos);  
  delay(1000);
  closeHand(closeHandPos);
  delay(1000);
  if(debugArmMovement)
  {
    armServo.write(20);
    delay(1000);
    slowMoveArm(20, 160, 1);
    delay(1000);
    openHand(openHandPos);
    delay(1000);
    slowMoveArm(160,20, 1);
    delay(1000);
    closeHand(closeHandPos);
  }

  armServo.write(90);  // return to neutral position
  Serial.println("Finished setup");
}


void loop()
{
 
  //get current nunchuck position data
  nunchuck_get_data();
  nunchuck_interp_data();

  //construct smoothed position data
  joyXs = wgtAvg(joyX, joyXs);
  joyYs = wgtAvg(joyY, joyYs);
  accXs = wgtAvg(accX, accXs);
  accYs = wgtAvg(accY, accYs);

  if (debugNumchuck) 
  {
    //Serial.println(" ");
    Serial.print("joy = "); 
    Serial.print(joyX); 
    Serial.print("."); 
    Serial.print(joyXs); 
    Serial.print("  "); 
    Serial.print(joyY);
    Serial.print("."); 
    Serial.print(joyYs); 
    Serial.print("\tacc = "); 
    Serial.print(accX); 
    Serial.print("."); 
    Serial.print(accXs); 
    Serial.print("  "); 
    Serial.print(accY);
    Serial.print("."); 
    Serial.print(accYs); 
    Serial.print("\tbut = "); 
    Serial.print(zButton); 
    Serial.print (" "); 
    Serial.print(cButton);
    Serial.print("\t");
  }

// move the arm
  if ( useJoystick == 1 )  {    
    armPos(joyXs);
  } else {
    armPos(accXs);
  }

// open and close the hand using the buttons
  if (cButton==1) { 
    closeHand(closeHandPos);
  } else { 
    openHand(openHandPos); 
  }

  Serial.println(" ");
  delay(20);
}

//
// Robotic Arm Functions
//

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

//function to quickly position the arm
void armPos(int pos)
{
  Serial.print("armPos (");
  Serial.print(pos);
  Serial.print(") ");
  armServo.write(pos);
  delay(delaySpeed);
}

//function to open the hand
void openHand(int pos)
{
  Serial.print("openHand (");
  Serial.print(pos);
  Serial.print(") ");
  handServo.write(pos);
  delay(delaySpeed);
}

//function to close the hand
void closeHand(int pos)
{
  Serial.print("closeHand (");
  Serial.print(pos);
  Serial.print(") ");
  handServo.write(pos);
  delay(delaySpeed);
}


// simple weighted average function for smoother positioning (returns integer)
int wgtAvg(int input, int old) {
  return wgtAvgSpeed*input + (1-wgtAvgSpeed)*old;
}


//
// Nunchuck functions
//

static uint8_t nunchuck_buf[6];   // array to store nunchuck data,

// Uses port C (analog in) pins as power & ground for Nunchuck
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
  DDRC |= _BV(pwrpin) | _BV(gndpin);
  PORTC &=~ _BV(gndpin);
  PORTC |=  _BV(pwrpin);
  delay(100);  // wait for things to stabilize        
}

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
void nunchuck_init()
{ 
  Wire.begin();	                // join i2c bus as master
  Wire.beginTransmission(0x52);	// transmit to device 0x52
  Wire.write(0x40);		// sends memory address
  Wire.write(0x00);		// sends sent a zero.  
  Wire.endTransmission();	// stop transmitting
}

// Send a request for data to the nunchuck
// was "send_zero()"
void nunchuck_send_request()
{
  Wire.beginTransmission(0x52);	// transmit to device 0x52
  Wire.write(0x00);		// sends one byte
  Wire.endTransmission();	// stop transmitting
}

// Receive data back from the nunchuck, 
int nunchuck_get_data()
{
  int cnt=0;
  Wire.requestFrom (0x52, 6);	// request data from nunchuck
  while (Wire.available ()) {
    // receive byte as an integer
    nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
    cnt++;
  }
  nunchuck_send_request();  // send request for next data payload
  // If we recieved the 6 bytes, then go print them
  if (cnt >= 5) {
    return 1;   // success
  }
  return 0; //failure
}

// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void nunchuck_interp_data()
{ 
  static int i=0;
  int jx = nunchuck_buf[0];
  int jy = nunchuck_buf[1];
  int accel_x_axis = nunchuck_buf[2]; // * 2 * 2; 
  int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
  int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;

  int z_button = 0;
  int c_button = 0;

  // byte nunchuck_buf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((nunchuck_buf[5] >> 0) & 1) 
    z_button = 1;
  if ((nunchuck_buf[5] >> 1) & 1)
    c_button = 1;

  if ((nunchuck_buf[5] >> 2) & 1) 
    accel_x_axis += 2;
  if ((nunchuck_buf[5] >> 3) & 1)
    accel_x_axis += 1;

  if ((nunchuck_buf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((nunchuck_buf[5] >> 5) & 1)
    accel_y_axis += 1;

  if ((nunchuck_buf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((nunchuck_buf[5] >> 7) & 1)
    accel_z_axis += 1;

  //  Serial.print(i,DEC);
  //  Serial.print("\t");

  // remap the joystick coordinates (46, 255) centered on 174 to (0, 180) centered on 90
  // no idea why the joystick reports an asymetric joystick
  if ( jx <= 174 )
  {
    joyX = (jx - 46)*90/(174-46);
  } 
  else {
    joyX = 90 + (jx - 174)*90/(255-174);
  }

  if ( jy <= 174 )
  {
    joyY = (jy - 46)*90/(174-46);
  } 
  else {
    joyY = 90 + (jy - 174)*90/(255-174);
  }

  joyX = constrain(joyX,0,180);  // no out of bounds values
  joyX = map(joyX,0,180,180,0);  // input from nunchuck is reversed ... fix it
  joyY = constrain(joyY,0,180); 

  // truncate and remap the accel data
  //  accX = map(constrain(accel_x_axis,90,180),90,180,0,180);
  accX = map(constrain(accel_x_axis,90,180),90,180,180,0);
  accY = map(constrain(accel_y_axis,90,180),90,180,0,180);

  // grab the button data
  zButton = nunchuck_zbutton();
  cButton = nunchuck_cbutton();

  i++;
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuk_decode_byte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_zbutton()
{
  return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_cbutton()
{
  return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}







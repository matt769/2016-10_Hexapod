#include <Wire.h>

const int servoslave_address=40; // I2C Address of Servo Chip
int Pos;
char cmd = '0'; // for serial input
char prevCmd = '0';

// assign servos
byte RIGHT_FRONT_H = 0;
byte RIGHT_FRONT_V = 1;
byte RIGHT_MID_H = 2;
byte RIGHT_MID_V = 3;
byte RIGHT_BACK_H = 4;
byte RIGHT_BACK_V = 5;
byte LEFT_FRONT_H = 6;
byte LEFT_FRONT_V = 7;
byte LEFT_MID_H = 8;
byte LEFT_MID_V = 9;
byte LEFT_BACK_H = 10;
byte LEFT_BACK_V = 11;

// store current positions (when run getPositions)
byte positions[12];

// store start/min/max positions for each leg
// because the body does now allow for much forward/backward movement, this range is very small (until I change the body)

byte V_RANGE = 62;
byte H_RANGE = 20;

byte RIGHT_FRONT_H_ST = 121;
byte RIGHT_FRONT_H_FW = RIGHT_FRONT_H_ST + H_RANGE;
byte RIGHT_FRONT_H_BW = RIGHT_FRONT_H_ST - H_RANGE;
byte RIGHT_FRONT_V_ST = 150;
byte RIGHT_FRONT_V_UP = RIGHT_FRONT_V_ST + V_RANGE;
byte RIGHT_FRONT_V_DN = RIGHT_FRONT_V_ST - V_RANGE;
byte RIGHT_MID_H_ST = 135;
byte RIGHT_MID_H_FW = RIGHT_MID_H_ST + H_RANGE;
byte RIGHT_MID_H_BW = RIGHT_MID_H_ST - H_RANGE;
byte RIGHT_MID_V_ST = 135;
byte RIGHT_MID_V_UP = RIGHT_MID_V_ST + V_RANGE;
byte RIGHT_MID_V_DN = RIGHT_MID_V_ST - V_RANGE;
byte RIGHT_BACK_H_ST = 125;
byte RIGHT_BACK_H_FW = RIGHT_BACK_H_ST + H_RANGE;
byte RIGHT_BACK_H_BW = RIGHT_BACK_H_ST - H_RANGE;
byte RIGHT_BACK_V_ST = 127;
byte RIGHT_BACK_V_UP = RIGHT_BACK_V_ST + V_RANGE; // horizontal 200
byte RIGHT_BACK_V_DN = RIGHT_BACK_V_ST - V_RANGE; // vertical 55
byte LEFT_FRONT_H_ST = 135;
byte LEFT_FRONT_H_FW = LEFT_FRONT_H_ST - H_RANGE;
byte LEFT_FRONT_H_BW = LEFT_FRONT_H_ST + H_RANGE;
byte LEFT_FRONT_V_ST = 140;
byte LEFT_FRONT_V_UP = LEFT_FRONT_V_ST - V_RANGE;
byte LEFT_FRONT_V_DN = LEFT_FRONT_V_ST + V_RANGE;
byte LEFT_MID_H_ST = 127;
byte LEFT_MID_H_FW = LEFT_MID_H_ST - H_RANGE;
byte LEFT_MID_H_BW = LEFT_MID_H_ST + H_RANGE;
byte LEFT_MID_V_ST = 145;
byte LEFT_MID_V_UP = LEFT_MID_V_ST - V_RANGE;
byte LEFT_MID_V_DN = LEFT_MID_V_ST + V_RANGE;
byte LEFT_BACK_H_ST = 127;
byte LEFT_BACK_H_FW = LEFT_BACK_H_ST - H_RANGE;
byte LEFT_BACK_H_BW = LEFT_BACK_H_ST + H_RANGE;
byte LEFT_BACK_V_ST = 127;
byte LEFT_BACK_V_UP = LEFT_BACK_V_ST - V_RANGE;
byte LEFT_BACK_V_DN = LEFT_BACK_V_ST + V_RANGE;




void servoConfig(int device, byte address, byte val) {
 Wire.beginTransmission(device); // start transmission to device
 Wire.write(address); // send register address
 Wire.write(val); // send value to write
 Wire.endTransmission(); // end transmission
} 

void servoStartup(int device) {  
  unsigned char i;  
  Wire.beginTransmission(device); // start transmission to device    
  Wire.write(0);                   // send starting register address   
  Wire.write(RIGHT_FRONT_H_ST);
  Wire.write(RIGHT_FRONT_V_ST);
  Wire.write(RIGHT_MID_H_ST);
  Wire.write(RIGHT_MID_V_ST);
  Wire.write(RIGHT_BACK_H_ST);
  Wire.write(RIGHT_BACK_V_ST);
  Wire.write(LEFT_FRONT_H_ST);
  Wire.write(LEFT_FRONT_V_ST);
  Wire.write(LEFT_MID_H_ST);
  Wire.write(LEFT_MID_V_ST);
  Wire.write(LEFT_BACK_H_ST);
  Wire.write(LEFT_BACK_V_ST);
  Wire.endTransmission(); // end transmission                               
}


void sendData(int device, byte address, int data) {  
  Wire.beginTransmission(device); // start transmission to device    
  Wire.write(address);                   // send register address   
  Wire.write(data); // send value for servo   
  Wire.endTransmission(); // end transmission                               
}


void stand() {
  sendData(servoslave_address, RIGHT_FRONT_V, RIGHT_FRONT_V_DN);
  sendData(servoslave_address, RIGHT_MID_V, RIGHT_MID_V_DN);
  sendData(servoslave_address, RIGHT_BACK_V, RIGHT_BACK_V_DN);
  sendData(servoslave_address, LEFT_FRONT_V, LEFT_FRONT_V_DN);
  sendData(servoslave_address, LEFT_MID_V, LEFT_MID_V_DN);
  sendData(servoslave_address, LEFT_BACK_V, LEFT_BACK_V_DN);
}



void attack(){
  sendData(servoslave_address, RIGHT_FRONT_H, 230);
  sendData(servoslave_address, RIGHT_FRONT_V, 220);
  sendData(servoslave_address, LEFT_FRONT_H, 40);
  sendData(servoslave_address, LEFT_FRONT_V, 40);
}


// this doesn't work properly, just returns all zeros
void getServoPositions(int device){
  int i=0;
   Wire.beginTransmission(device); // start transmission to device
   Wire.write(0); // write register address (servo number)
   Wire.endTransmission(); // end transmission
   while(Wire.available()) {
     positions[i] = Wire.read(); // read a byte as character
     i++;
   }

}


// get single servo position
unsigned char getservoPos(int device, byte servo_num) {
 unsigned char servoPosition;

 //write the Servo number to read
 Wire.beginTransmission(device); // start transmission to device
 Wire.write(servo_num); // write register address (servo number)
 Wire.endTransmission(); // end transmission

 //Request servo position
 Wire.requestFrom(device, 1); // request 1 byte from slave device
 if(Wire.available())
 {
 servoPosition = Wire.read(); // read a byte as character
 }
 return servoPosition;
}



int offset(int base, int range, int offset){
  int mod = 0;
  if ((base + offset) >= range) {
    mod = range;
  }
  return base + offset - mod;
}



void setOneUp(int wait) {
  // set one is front left, mid right, back left
  sendData(servoslave_address, LEFT_FRONT_V, LEFT_FRONT_V_UP);
  delay(wait);
  sendData(servoslave_address, RIGHT_MID_V, RIGHT_MID_V_UP);
  delay(wait);
  sendData(servoslave_address, LEFT_BACK_V, LEFT_BACK_V_UP);
  delay(wait);
}
void setOneDn(int wait) {
  // set one is front left, mid right, back left
  sendData(servoslave_address, LEFT_FRONT_V, LEFT_FRONT_V_DN);
  delay(wait);
  sendData(servoslave_address, RIGHT_MID_V, RIGHT_MID_V_DN);
  delay(wait);
  sendData(servoslave_address, LEFT_BACK_V, LEFT_BACK_V_DN);
  delay(wait);
}
void setOneFw(int wait) {
  // set one is front left, mid right, back left
  sendData(servoslave_address, LEFT_FRONT_H, LEFT_FRONT_H_FW);
  delay(wait);
  sendData(servoslave_address, RIGHT_MID_H, RIGHT_MID_H_FW);
  delay(wait);
  sendData(servoslave_address, LEFT_BACK_H, LEFT_BACK_H_FW);
  delay(wait);
}
void setOneBw(int wait) {
  // set one is front left, mid right, back left
  sendData(servoslave_address, LEFT_FRONT_H, LEFT_FRONT_H_BW);
  delay(wait);
  sendData(servoslave_address, RIGHT_MID_H, RIGHT_MID_H_BW);
  delay(wait);
  sendData(servoslave_address, LEFT_BACK_H, LEFT_BACK_H_BW);
  delay(wait);
}



void setTwoUp(int wait) {
  // set two is front left, mid right, back left legs
  sendData(servoslave_address, RIGHT_FRONT_V, RIGHT_FRONT_V_UP);
  delay(wait);
  sendData(servoslave_address, LEFT_MID_V, LEFT_MID_V_UP);
  delay(wait);
  sendData(servoslave_address, RIGHT_BACK_V, RIGHT_BACK_V_UP);
  delay(wait);
}
void setTwoDn(int wait) {
  // set two is front left, mid right, back left legs
  sendData(servoslave_address, RIGHT_FRONT_V, RIGHT_FRONT_V_DN);
  delay(wait);
  sendData(servoslave_address, LEFT_MID_V, LEFT_MID_V_DN);
  delay(wait);
  sendData(servoslave_address, RIGHT_BACK_V, RIGHT_BACK_V_DN);
  delay(wait);
}
void setTwoFw(int wait) {
  // set two is front left, mid right, back left legs
  sendData(servoslave_address, RIGHT_FRONT_H, RIGHT_FRONT_H_FW);
  delay(wait);
  sendData(servoslave_address, LEFT_MID_H, LEFT_MID_H_FW);
  delay(wait);
  sendData(servoslave_address, RIGHT_BACK_H, RIGHT_BACK_H_FW);
  delay(wait);
}
void setTwoBw(int wait) {
  // set two is front left, mid right, back left legs
  sendData(servoslave_address, RIGHT_FRONT_H, RIGHT_FRONT_H_BW);
  delay(wait);
  sendData(servoslave_address, LEFT_MID_H, LEFT_MID_H_BW);
  delay(wait);
  sendData(servoslave_address, RIGHT_BACK_H, RIGHT_BACK_H_BW);
  delay(wait);
}

void setThreeBw() {
  // set three is all the horizontal ones
  sendData(servoslave_address, RIGHT_FRONT_H, RIGHT_FRONT_H_BW);
  sendData(servoslave_address, LEFT_FRONT_H, LEFT_FRONT_H_BW);
  sendData(servoslave_address, RIGHT_MID_H, RIGHT_MID_H_BW);
  sendData(servoslave_address, LEFT_MID_H, LEFT_MID_H_BW);
  sendData(servoslave_address, RIGHT_BACK_H, RIGHT_BACK_H_BW);
  sendData(servoslave_address, LEFT_BACK_H, LEFT_BACK_H_BW);
}


void moveForwardTripod(int wait){

  setOneFw(0);
  delay(wait);
  setOneDn(0);
  delay(wait);
  setTwoUp(0);
  delay(wait);
  setOneBw(0);
  delay(wait);
  setTwoFw(0);
  delay(wait);
  setTwoDn(0);
  delay(wait);
  setOneUp(0);
  delay(wait);
  setTwoBw(0);
  delay(wait);

}



void moveGeneral(int wait, int DIRECTION, int RANGE, int RAISE_POINT, int LOWER_POINT, 
                  int OFFSET_LF, int OFFSET_LM, int OFFSET_LB, int OFFSET_RF, int OFFSET_RM, int OFFSET_RB){
  int j;

  if (DIRECTION == 1) {
    
  for (j=0;j<180;j++) {
    Serial.println(j);
    if (offset(j,RANGE,OFFSET_RF) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RF) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_DN);
    }
    sendData(servoslave_address, RIGHT_FRONT_H, map(offset(j,RANGE,OFFSET_RF),0,RANGE,RIGHT_FRONT_H_FW,RIGHT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_LF) > RAISE_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LF) < LOWER_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_DN);
    }
    sendData(servoslave_address, LEFT_FRONT_H, map(offset(j,RANGE,OFFSET_LF),0,RANGE,LEFT_FRONT_H_FW,LEFT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_RM) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RM) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_DN);
    }
    sendData(servoslave_address, RIGHT_MID_H, map(offset(j,RANGE,OFFSET_RM),0,RANGE,RIGHT_MID_H_FW,RIGHT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_LM) > RAISE_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LM) < LOWER_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_DN);
    }
    sendData(servoslave_address, LEFT_MID_H, map(offset(j,RANGE,OFFSET_LM),0,RANGE,LEFT_MID_H_FW,LEFT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_RB) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RB) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_DN);
    }
    sendData(servoslave_address, RIGHT_BACK_H, map(offset(j,RANGE,OFFSET_RB),0,RANGE,RIGHT_BACK_H_FW,RIGHT_BACK_H_BW));
    
    if (offset(j,RANGE,OFFSET_LB) > RAISE_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LB) < LOWER_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_DN);
    }
    sendData(servoslave_address, LEFT_BACK_H, map(offset(j,RANGE,OFFSET_LB),0,RANGE,LEFT_BACK_H_FW,LEFT_BACK_H_BW));
    
    delay(wait);
    
  }
  }
  // DOESN'T WORK - have switched raise/lower points in below
  // but seems like it's not the same but backwards
  // *sounds* different but can't see what is different
  if (DIRECTION == -1) {
    
  for (j=RANGE;j>0;j--) {
    Serial.println(j);
    if (offset(j,RANGE,OFFSET_RF) > LOWER_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RF) < RAISE_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_DN);
    }
    sendData(servoslave_address, RIGHT_FRONT_H, map(offset(j,RANGE,OFFSET_RF),0,RANGE,RIGHT_FRONT_H_FW,RIGHT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_LF) > LOWER_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LF) < RAISE_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_DN);
    }
    sendData(servoslave_address, LEFT_FRONT_H, map(offset(j,RANGE,OFFSET_LF),0,RANGE,LEFT_FRONT_H_FW,LEFT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_RM) > LOWER_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RM) < RAISE_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_DN);
    }
    sendData(servoslave_address, RIGHT_MID_H, map(offset(j,RANGE,OFFSET_RM),0,RANGE,RIGHT_MID_H_FW,RIGHT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_LM) > LOWER_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LM) < RAISE_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_DN);
    }
    sendData(servoslave_address, LEFT_MID_H, map(offset(j,RANGE,OFFSET_LM),0,RANGE,LEFT_MID_H_FW,LEFT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_RB) > LOWER_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RB) < RAISE_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_DN);
    }
    sendData(servoslave_address, RIGHT_BACK_H, map(offset(j,RANGE,OFFSET_RB),0,RANGE,RIGHT_BACK_H_FW,RIGHT_BACK_H_BW));
    
    if (offset(j,RANGE,OFFSET_LB) > LOWER_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LB) < RAISE_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_DN);
    }
    sendData(servoslave_address, LEFT_BACK_H, map(offset(j,RANGE,OFFSET_LB),0,RANGE,LEFT_BACK_H_FW,LEFT_BACK_H_BW));
    
    delay(wait);
    
  }
  }

}

// for the moment I will use DIRECTION to represent left or right
// need to shorten the range on the inner side
// incorporate into the main move function
// FUNCTION NOT COMPLETE!
void moveTurn(int wait, int DIRECTION, int RANGE, int RAISE_POINT, int LOWER_POINT, 
                  int OFFSET_LF, int OFFSET_LM, int OFFSET_LB, int OFFSET_RF, int OFFSET_RM, int OFFSET_RB){
  int j;
  int LEFT_DIV;
  int RIGHT_DIV;

  // left
  switch (DIRECTION) {
    case 1:
      LEFT_DIV = 2;
      RIGHT_DIV = 1;
      break;
    case -1:
      LEFT_DIV = 1;
      RIGHT_DIV = 2;
      break;
  }
    
  for (j=0;j<180;j++) {
    Serial.println(j);
    if (offset(j,RANGE,OFFSET_RF) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RF) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_FRONT_V,RIGHT_FRONT_V_DN);
    }
    sendData(servoslave_address, RIGHT_FRONT_H, map(offset(j,RANGE,OFFSET_RF)/RIGHT_DIV,0,RANGE,RIGHT_FRONT_H_FW,RIGHT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_LF) > RAISE_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LF) < LOWER_POINT){
      sendData(servoslave_address, LEFT_FRONT_V,LEFT_FRONT_V_DN);
    }
    sendData(servoslave_address, LEFT_FRONT_H, map(offset(j,RANGE,OFFSET_LF)/LEFT_DIV,0,RANGE,LEFT_FRONT_H_FW,LEFT_FRONT_H_BW));
    
    if (offset(j,RANGE,OFFSET_RM) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RM) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_MID_V,RIGHT_MID_V_DN);
    }
    sendData(servoslave_address, RIGHT_MID_H, map(offset(j,RANGE,OFFSET_RM)/RIGHT_DIV,0,RANGE,RIGHT_MID_H_FW,RIGHT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_LM) > RAISE_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LM) < LOWER_POINT){
      sendData(servoslave_address, LEFT_MID_V,LEFT_MID_V_DN);
    }
    sendData(servoslave_address, LEFT_MID_H, map(offset(j,RANGE,OFFSET_LM)/LEFT_DIV,0,RANGE,LEFT_MID_H_FW,LEFT_MID_H_BW));
    
    if (offset(j,RANGE,OFFSET_RB) > RAISE_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_RB) < LOWER_POINT){
      sendData(servoslave_address, RIGHT_BACK_V,RIGHT_BACK_V_DN);
    }
    sendData(servoslave_address, RIGHT_BACK_H, map(offset(j,RANGE,OFFSET_RB)/RIGHT_DIV,0,RANGE,RIGHT_BACK_H_FW,RIGHT_BACK_H_BW));
    
    if (offset(j,RANGE,OFFSET_LB) > RAISE_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_UP);
    }
    if (offset(j,RANGE,OFFSET_LB) < LOWER_POINT){
      sendData(servoslave_address, LEFT_BACK_V,LEFT_BACK_V_DN);
    }
    sendData(servoslave_address, LEFT_BACK_H, map(offset(j,RANGE,OFFSET_LB)/LEFT_DIV,0,RANGE,LEFT_BACK_H_FW,LEFT_BACK_H_BW));
    
    delay(wait);
    
  }

}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    cmd = (char)Serial.read();
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting..."));
  Wire.begin(); // join i2c bus (address optional for master)
  servoConfig(servoslave_address, 62, 0);   // set freq to 50Hz
  servoStartup(servoslave_address); 
  servoConfig(servoslave_address, 60, 1);   // all outputs on
 
  }

 
void loop() {


    stand();
    delay(1000);
    moveForwardTripod(100);  // tripod
    moveForwardTripod(100);
    moveForwardTripod(100);
    moveForwardTripod(100);
    moveForwardTripod(100);
    delay(1000);
    moveGeneral(5, 1, 180, 170, 10, 30, 90, 150, 0, 60, 120);  // original ripple
    moveGeneral(5, 1, 180, 170, 10, 30, 90, 150, 0, 60, 120);
    moveGeneral(5, 1, 180, 170, 10, 30, 90, 150, 0, 60, 120);
    moveGeneral(5, 1, 180, 170, 10, 30, 90, 150, 0, 60, 120);
    moveGeneral(5, 1, 180, 170, 10, 30, 90, 150, 0, 60, 120);
    delay(1000);
    moveGeneral(5, 1, 180, 170, 10, 60, 30, 0, 150, 120, 90);  // wave
    moveGeneral(5, 1, 180, 170, 10, 60, 30, 0, 150, 120, 90);
    moveGeneral(5, 1, 180, 170, 10, 60, 30, 0, 150, 120, 90);
    moveGeneral(5, 1, 180, 170, 10, 60, 30, 0, 150, 120, 90);
    moveGeneral(5, 1, 180, 170, 10, 60, 30, 0, 150, 120, 90);
    delay(1000);
    moveGeneral(5, 1, 180, 170, 10, 120, 60, 0, 90, 30, 150);  // ripple 2
    moveGeneral(5, 1, 180, 170, 10, 120, 60, 0, 90, 30, 150);
    moveGeneral(5, 1, 180, 170, 10, 120, 60, 0, 90, 30, 150);
    moveGeneral(5, 1, 180, 170, 10, 120, 60, 0, 90, 30, 150);
    moveGeneral(5, 1, 180, 170, 10, 120, 60, 0, 90, 30, 150);
    delay(1000);
    moveGeneral(5, 1, 180, 170, 10, 90, 30, 150, 0, 120, 60);  // ripple 3
    moveGeneral(5, 1, 180, 170, 10, 90, 30, 150, 0, 120, 60);
    moveGeneral(5, 1, 180, 170, 10, 90, 30, 150, 0, 120, 60);
    moveGeneral(5, 1, 180, 170, 10, 90, 30, 150, 0, 120, 60);
    moveGeneral(5, 1, 180, 170, 10, 90, 30, 150, 0, 120, 60);
    delay(1000);
    

 }




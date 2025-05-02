// C++ code
//
#include <Servo.h>

Servo myservo; //create a servo object

void setup() {
  myservo.attach(9); //servo is connected to pin 9
}

void  loop () {
  myservo.write(90); //move the servo to servoPosition
  
}
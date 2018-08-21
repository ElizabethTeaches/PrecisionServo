/*
 * Runs two servos of the servo manager.
 * 
 * Servos should be connected to pins 0 and 1 and should 
 * be run through a voltage regulator.
 * 
 * Cycles each servo through each of the 9 settings at the 
 * same rate, except the servo on pin 1 is 4 settings "ahead"
 * of the servo on pin 0. 
 * 
 * For showing the setting, the servos go to 0, then to 180,
 * then to 90, then pause and increment their setting.
 * 
 * When the servo seems to pause at the edges it's likely at 
 * either a precise or endpoint setting. When it seems to skip
 * the endpoint (often seeming to lose some range of motion)
 * it is on one of the smooth settings.
 */

#include <ServoManager.h>

const int servoPin1 = 0;
const int servoPin2 = 1;

int setting = 0;
PrecisionServo* servo1;
PrecisionServo* servo2;

void setup() {
  ServoManager::setup(2);

  servo1 = ServoManager::createServo(servoPin1);
  servo1->setSetting(setting);

  servo2 = ServoManager::createServo(servoPin2);
  servo2->setSetting((setting + 4) % 9);
} 

void loop() {
  servo1->setTarget(0);
  servo2->setTarget(0);
  ServoManager::waitUntilFinished();

  servo1->setTarget(180);
  servo2->setTarget(180);
  ServoManager::waitUntilFinished();

  servo1->setTarget(90);
  servo2->setTarget(90);
  ServoManager::waitUntilFinished();
  delay(500);

  setting = (setting + 1) % 9;
  servo1->setSetting(setting);
  servo2->setSetting((setting + 4) % 9);
}

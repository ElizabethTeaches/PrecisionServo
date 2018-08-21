/**
 * Sweeps a single servo on the medium endoint setting from 0 to 180.
 */

#include <ServoManager.h>

const int servoPin = 0;

PrecisionServo* servo;

void setup() {
  ServoManager::setup(1);

  servo = ServoManager::createServo(servoPin);
  servo->setSetting(SERVO_MEDIUM_ENDPOINT);
}

void loop() {
  servo->setTarget(0);
  ServoManager::waitUntilFinished();

  servo->setTarget(180);
  ServoManager::waitUntilFinished();
}

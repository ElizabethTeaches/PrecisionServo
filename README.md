# PrecisionServo

PrecisionServo provides an alternative to [SoftwareServo](https://github.com/nicolaskruchten/arduino/tree/master/libraries/SoftwareServo) for ATTiny's which do not work with the Arduino built-in servo library. This library was tested with an ATTiny85 via a usb programmer on an Arduino IDE with the Tower Pro Micro Servo 9g (an SG90 servo). The servos positive power should first go through a voltage regulator (I use L7805CV - a 5 volt regulator).

This library uses the Timer/Counter1 in order to control the servo. Thus, when using this library, you cannot edit TIMSK bit TOIE1, TCCR1, OCR1C, or PLLCSR bits related to timer1, or any other settings that would modify the behavior of those registry masks.

## Simple example 

This is from single_server_sweep in the examples:

```cpp
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
```

## Usage explained

The servo manager should be setup with the number of unique servos on the circuit first, with `ServoManager::setup`. Then, create each servo one at a time by calling `ServoManager::createServo` with the corresponding pin. The returned servo is ready to go, however you might want to change the setting (see Settings below) using `PrecisionServo->setSetting`. Every servo may have its own setting.

From there, call setTarget on the servo with the target direction in degrees. Immediately after this function call the servo manager will start to move the servo in that direction, interrupting any running code in order to do so (so it acts kind-of asynchronously from your perspective). You might want to wait until the servo actually reaches its target before moving on, which can be done by calling `ServoManager::waitUntilFinished`. You can change the target at any time, even prior to it reaching its old target, and the servo manager will automatically start moving the servo in that direction.

Targets are *not* queued, so if you change the target before it reaches the destination, the servo range will appear to be clipped.

Since the servo manager works in the background, you can set a different servos target while a servo is moving. See examples/two_servos_settings_sweep.ino

## Settings 

There are many different modes of operation that you can set on the servos via `PrecisionServo->setSetting`.

### Constants

- `SERVO_SLOW_SMOOTH`
- `SERVO_SLOW_PRECISE`
- `SERVO_SLOW_ENDPOINT`
- `SERVO_MEDIUM_SMOOTH` 
- `SERVO_MEDIUM_PRECISE`
- `SERVO_MEDIUM_ENDPOINT`
- `SERVO_FAST_SMOOTH`
- `SERVO_FAST_PRECISE`
- `SERVO_FAST_ENDPOINT`

SLOW/MEDIUM/FAST controls how rapidly the servo is going to the target 
 
- **SMOOTH**: No additional pauses are used; the servo chases the target but often won't quite reach it. This is great for if you are trying to follow a sensor and the angles your asking for are inherently inprecise.
- **PRECISE**: The servo pauses at the *old* target whenever the target changes. This helps ensure it makes it exactly to the requested angles, assuming that you are using waitUntilFinished or sufficient delays between changing targets. This is ideal if you are hardcoding a sequence of movements and want the most consistent behavior possible.
- **ENDPOINT**: Acts very similiarly to PRECISE, except skips the pause when given orders that continue in the same direction (the servo is often less accurate when changing directions)

## Calibration

Depending on the exact amount of resistance on your circuit and the imperfections in the servos themself, they might not act exactly like you expect on the default settings. If the range seems off, verify no wires are loose, that a voltage regulator is in place, and that your batteries are charged (if powered off batteries) and then modify the MICROSECONDS_FOR_180 and MICROSECONDS_FOR_0 definitions at the top of ServoManager.cpp.
#include "Motor.hpp"

Motor::Motor() {}

Motor::Motor(int in1Pin, int in2Pin, int enablePin)
    : in1Pin(in1Pin), in2Pin(in2Pin), enablePin(enablePin) {
  stop();
}

void Motor::setPin(Pin pin, int value) {
  switch (pin) {
    case IN1:
      in1Pin = value;
      break;
    case IN2:
      in2Pin = value;
      break;
    case ENABLE:
      enablePin = value;
      break;
  }
}

void Motor::_setSpeed(int speed) { this->speed = speed; }

int Motor::getSpeed() { return speed; }

void Motor::setDirection(Direction direction) {
  this->direction = direction;
  switch (direction) {
    case FORWARD:
      digitalWrite(in1Pin, HIGH);
      digitalWrite(in2Pin, LOW);
      break;
    case BACKWARD:
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
      break;
    case STOP:
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
      break;
  }
}

int Motor::getDirection() { return direction; }

void Motor::stop() {
  setDirection(STOP);
  _setSpeed(0);
  analogWrite(enablePin, 0);
}

void Motor::accelerate(int rate = 5) {
  int newSpeed = getSpeed() + rate;
  if (newSpeed > 255) {
    newSpeed = 255;
  } else if (newSpeed < 0) {
    newSpeed = 0;
  }
  _setSpeed(newSpeed);
  analogWrite(enablePin, newSpeed);
}

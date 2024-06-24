#include "Ultrasonic.hpp"

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, int maxDistance) {
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;
  this->maxDistance = maxDistance;
  sonar = new NewPing(triggerPin, echoPin, maxDistance);
}

int Ultrasonic::getDistance() { return sonar->ping_cm(); }

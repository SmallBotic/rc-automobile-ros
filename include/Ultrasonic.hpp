#ifndef ULTRASONIC_HPP_
#define ULTRASONIC_HPP_

#include <Arduino.h>
#include <NewPing.h>

#include "Config.hpp"

class Ultrasonic {
 private:
  int triggerPin;
  int echoPin;
  int maxDistance = ULTRASONIC_MAX_DISTANCE;
  NewPing *sonar;

 public:
  /**
   * @brief Construct a new Ultrasonic object
   * @param triggerPin The pin connected to the trigger pin of the ultrasonic
   * sensor
   * @param echoPin The pin connected to the echo pin of the ultrasonic sensor
   * @param maxDistance The maximum distance the ultrasonic sensor can measure
   */
  Ultrasonic(int triggerPin, int echoPin,
             int maxDistance = ULTRASONIC_MAX_DISTANCE);

  /**
   * @brief Get the distance from the ultrasonic sensor
   * @return int The distance in centimeters
   */
  int getDistance();
};

#endif  // ULTRASONIC_HPP_
#ifndef MOTOR_HPP_
#define MOTOR_HPP_

#include <Arduino.h>

class Motor
{
private:
  int enablePin;
  int in1Pin;
  int in2Pin;
  int speed;
  int direction;
  /**
   * @brief Decelerate to the given speed
   * @param rate The rate of deceleration
   */
  void _decelerate(int rate = 5);

  /**
   * @brief Set the speed of the motor
   * @param speed The speed to set the motor to
   */
  void _setSpeed(int speed);

 public:
  enum Direction
  {
    BACKWARD = -1,
    STOP = 0,
    FORWARD = 1,
  };

  enum Pin
  {
    IN1 = 0,
    IN2 = 1,
    ENABLE = 2,
  };

  /**
   * @brief Construct a new Motor object
   */
  Motor();

  /**
   * @brief Construct a new Motor object
   * @param in1Pin The pin connected to the IN1 pin of the motor driver
   * @param in2Pin The pin connected to the IN2 pin of the motor driver
   * @param enablePin The pin connected to the ENABLE pin of the motor driver
   */
  Motor(int in1Pin, int in2Pin, int enablePin);

  /**
   * @brief Set the pin value
   * @param pin The pin to set
   * @param value The pin number to set to the pin
   */
  void setPin(Pin pin, int value);

  /**
   * @brief Get the speed of the motor
   * @return int The speed of the motor
   */
  int getSpeed();

  /**
   * @brief Set the direction of the motor
   * @param direction The direction to set the motor to
   */
  void setDirection(Direction direction);

  /**
   * @brief Get the direction of the motor
   * @return Direction The direction of the motor
   */
  int getDirection();

  /**
   * @brief Stop the motor
   */
  void stop();

  /**
   * @brief Accelerate the motor to the given speed
   * @param rate The rate of acceleration. (-rate) for deceleration
   */
  void accelerate(int rate);
};

#endif  // MOTOR_HPP_
#ifndef NAVIGATION_SYSTEM_HPP_
#define NAVIGATION_SYSTEM_HPP_

#include <Arduino.h>
#include <MPU6500_WE.h>
#include <QMC5883LCompass.h>

#include "Config.hpp"

class NavigationSystem : private MPU6500_WE, private QMC5883LCompass {
 public:
  struct Vector3f {
    float x;
    float y;
    float z;
  };

  struct Vector3Angles {
    float roll;
    float pitch;
    float yaw;
  };

  struct Vector3i {
    int x;
    int y;
    int z;
  };

 private:
  bool _magnetometerPresent = false;
  uint8_t _qmc5883Address;
  uint8_t _mpu6500Address;

  Vector3f _acceleration;
  Vector3f _gyroscope;
  float _temperature;
  Vector3Angles _angle;

  Vector3i _magneticField;
  int _heading;

  MPU6500_WE *mpu6500;
  QMC5883LCompass *qmc5883;

 public:
  /**
   * @brief Construct a new Navigation System object
   * @param mpu6500Address The address of the MPU6500 IMU
   */
  NavigationSystem(uint8_t mpu6500Address);

  ~NavigationSystem() {
    delete mpu6500;
    delete qmc5883;
  }

  /**
   * @brief Construct a new Navigation System object
   * @param mpu6500Address The address of the MPU6500 IMU
   * @param qmc5883Address The address of the QMC5883 magnetometer
   */
  NavigationSystem(uint8_t mpu6500Address, uint8_t qmc5883Address);

  /**
   * @brief Initialize the navigation system. Should be called in the setup
   */
  void initNS();

  /**
   * @brief Read the navigation sensors. This will depend on the sensors
   * present. Should be called in the loop
   */
  void read();

  /**
   * @brief Get the acceleration of the MPU6500
   * @return Vector3f The acceleration in m/s^2
   */
  Vector3f getAcceleration();

  /**
   * @brief Get the gyroscope data of the MPU6500
   * @return Vector3f The gyroscope data in rad/s
   */
  Vector3f getGyroscope();

  /**
   * @brief Get the heading of the QMC5883
   * @warning This function is not working properly
   * @return int The heading in degrees
   */
  int getHeading();

  /**
   * @brief Get the magnetic field of the QMC5883
   * @return Vector3i The magnetic field in uT
   */
  Vector3i getMagneticField();

  /**
   * @brief Get the temperature of the MPU6500
   * @return float The temperature in degrees Celsius
   */
  float getTemperature();

  /**
   * @brief Get the angle of the MPU6500
   * @return float The angle in degrees
   */
  Vector3Angles getAngle();

  /**
   * @brief Combine all the navigation data
   * @return String The stringified version of the navigation data
   */
  String stringify();
};

#endif  // NAVIGATION_SYSTEM_HPP_
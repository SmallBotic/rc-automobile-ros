#include "NavigationSystem.hpp"

NavigationSystem::NavigationSystem(uint8_t mpu6500Address) {
  this->_mpu6500Address = mpu6500Address;
}

NavigationSystem::NavigationSystem(uint8_t mpu6500Address,
                                   uint8_t qmc5883Address) {
  _magnetometerPresent = true;
  _mpu6500Address = mpu6500Address;
  _qmc5883Address = qmc5883Address;
}

void NavigationSystem::initNS() {
  mpu6500 = new MPU6500_WE(_mpu6500Address);
  mpu6500->init();
  mpu6500->autoOffsets();

  mpu6500->enableGyrDLPF();
  mpu6500->setGyrDLPF(MPU6500_DLPF_6);
  mpu6500->setGyrRange(MPU6500_GYRO_RANGE_250);

  mpu6500->setAccRange(MPU6500_ACC_RANGE_2G);
  mpu6500->enableAccDLPF(true);
  mpu6500->setAccDLPF(MPU6500_DLPF_6);

  mpu6500->setSampleRateDivider(5);

  if (_magnetometerPresent) {
    qmc5883 = new QMC5883LCompass();
    qmc5883->init();
    qmc5883->setMagneticDeclination(0, 39);
    // qmc5883->setSmoothing(10, false);
    qmc5883->calibrate();
  }
}

NavigationSystem::Vector3f NavigationSystem::getAcceleration() {
  return _acceleration;
}

NavigationSystem::Vector3f NavigationSystem::getGyroscope() {
  return _gyroscope;
}

float NavigationSystem::getTemperature() { return mpu6500->getTemperature(); }

NavigationSystem::Vector3Angles NavigationSystem::getAngle() { return _angle; }

void NavigationSystem::read() {
  xyzFloat acc = mpu6500->getGValues();
  _acceleration = {acc.x, acc.y, acc.z};

  xyzFloat gyro = mpu6500->getGyrValues();
  _gyroscope = {gyro.x, gyro.y, gyro.z};

  _temperature = mpu6500->getTemperature();

  _angle = {mpu6500->getRoll(), mpu6500->getPitch(), 0};

  if (_magnetometerPresent) {
    qmc5883->read();
    _magneticField = {qmc5883->getX(), qmc5883->getY(), qmc5883->getZ()};
    _heading = qmc5883->getAzimuth();
  }
}

int NavigationSystem::getHeading() { return _heading; }

NavigationSystem::Vector3i NavigationSystem::getMagneticField() {
  return _magneticField;
}

String NavigationSystem::stringify() {
  return String(_acceleration.x) + "," + String(_acceleration.y) + "," +
         String(_acceleration.z) + "," + String(_gyroscope.x) + "," +
         String(_gyroscope.y) + "," + String(_gyroscope.z) + "," +
         String(_magneticField.x) + "," + String(_magneticField.y) + "," +
         String(_magneticField.z);
}
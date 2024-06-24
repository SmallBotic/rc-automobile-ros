#include <Arduino.h>
#include <Wire.h>

#include <Motor.hpp>
#include <NavigationSystem.hpp>
#include <Ultrasonic.hpp>

Motor leftMotor(LEFT_MOTOR_IN1, LEFT_MOTOR_IN2, LEFT_MOTOR_ENABLE);
Motor rightMotor(RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2, RIGHT_MOTOR_ENABLE);
NavigationSystem nav(MPU6500_ADDRESS, QMC5883_ADDRESS);
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO);

void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  Wire.begin();
  nav.initNS();

  DEBUG_PRINTLN("Motors initialized");
}

void loop() {
  nav.read();
  int dist = ultrasonic.getDistance();
  
  DEBUG_PRINTF(
      "Ac: %f, %f, %f Gy: %f, %f, %f T: %f rpy: %f, %f, %f H: %d M: "
      "%d, %d, %d D: %d\n",
      nav.getAcceleration().x, nav.getAcceleration().y, nav.getAcceleration().z,
      nav.getGyroscope().x, nav.getGyroscope().y, nav.getGyroscope().z,
      nav.getTemperature(), nav.getAngle().roll, nav.getAngle().pitch,
      nav.getAngle().yaw, nav.getHeading(), nav.getMagneticField().x,
      nav.getMagneticField().y, nav.getMagneticField().z, dist);

  delay(100);
}
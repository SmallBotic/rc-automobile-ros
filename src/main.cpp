#include <Arduino.h>
#include <Wire.h>

#include <Credentials.hpp>
#include <MobileRos.hpp>
#include <Motor.hpp>
#include <NavigationSystem.hpp>
#include <Ultrasonic.hpp>

Motor leftMotor(LEFT_MOTOR_IN1, LEFT_MOTOR_IN2, LEFT_MOTOR_ENABLE);
Motor rightMotor(RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2, RIGHT_MOTOR_ENABLE);
NavigationSystem nav(MPU6500_ADDRESS, QMC5883_ADDRESS);
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO);

MicroROS uros;

long prevPublishTime = 0;

void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }
  Wire.begin();
  nav.initNS();

  uros.init();
}

void loop() {
  nav.read();
  int distance = ultrasonic.getDistance();
  String imu = nav.stringify();

  if (millis() - prevPublishTime > 100) {
    uros.publish(MicroROS::DISTANCE, distance);
    uros.publish(MicroROS::IMU, imu);
  }

  uros.spin();
}
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#define DEBUG 1
#define DEBUG_PRINT(x) \
  if (DEBUG) Serial.print(x)
#define DEBUG_PRINTLN(x) \
  if (DEBUG) Serial.println(x)
#define DEBUG_PRINTF(...) \
  if (DEBUG) Serial.printf(__VA_ARGS__)

// Motors
#define LEFT_MOTOR_IN1 27
#define LEFT_MOTOR_IN2 36
#define LEFT_MOTOR_ENABLE 33

#define RIGHT_MOTOR_IN1 18
#define RIGHT_MOTOR_IN2 19
#define RIGHT_MOTOR_ENABLE 25

// Ultrasonic
#define ULTRASONIC_TRIGGER 16
#define ULTRASONIC_ECHO 17
#define ULTRASONIC_MAX_DISTANCE 400

// MPU6500
#define MPU6500_SDA 21
#define MPU6500_SCL 22
#define MPU6500_ADDRESS 0x68

// QMC5883
#define QMC5883_ADDRESS 0x0D

// ROS
#define MICRO_ROS_ERROR_TIMEOUT 500
#define MICRO_ROS_NODE_NAME "rc_automobile"
#define MICRO_ROS_NODE_NAMESPACE ""
#define MICRO_ROS_CMD_VEL_SUBSCRIPTION_TOPIC "/rc_automobile_ros/cmd_vel"
#define MICRO_ROS_IMU_PUBLISH_TOPIC "/rc_automobile_ros/imu"
#define MICRO_ROS_DISTANCE_PUBLISH_TOPIC "/rc_automobile_ros/distance"

#endif  // CONFIG_HPP_
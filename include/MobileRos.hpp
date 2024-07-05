#ifndef MOBILE_ROSS_HPP_
#define MOBILE_ROSS_HPP_

#include <Arduino.h>
#include <geometry_msgs/msg/twist.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/string.h>

#include <Config.hpp>
#include <Credentials.hpp>

class MicroROS {
 private:
  rcl_subscription_t cmdVelSubscriber;
  rcl_publisher_t distancePublisher, imuPublisher;

  std_msgs__msg__Int32 distanceMsg;
  String imuMsg;
  geometry_msgs__msg__Twist controlMsg;

  rclc_executor_t executor;
  rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;
  rcl_timer_t timer;

  long previousErrorTime;

  // Error handle loop
  void error_loop();

#define RCSOFTCHECK(fn)                     \
  {                                         \
    rcl_ret_t temp_rc = fn;                 \
    if ((temp_rc != RCL_RET_OK)) {          \
      DEBUG_PRINTF("Error: %d\n", temp_rc); \
    }                                       \
  }

#define RCCHECK(fn)                \
  {                                \
    rcl_ret_t temp_rc = fn;        \
    if ((temp_rc != RCL_RET_OK)) { \
      error_loop();                \
    }                              \
  }

 public:
  enum PublisherType { DISTANCE, IMU };

 public:
  MicroROS();
  ~MicroROS();
  void init();
  void publish(MicroROS::PublisherType type, int data);
  void publish(MicroROS::PublisherType type, String data);
  void spin();
  rcl_publisher_t* getPublisher(MicroROS::PublisherType type);

  static void cmdVelSubCallback(const void* msgIn);
};

#endif  // MOBILE_ROSS_HPP_
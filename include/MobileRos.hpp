#ifndef MOBILE_ROSS_HPP_
#define MOBILE_ROSS_HPP_

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/int32.h>

#include <Config.hpp>
#include <Credentials.hpp>

class MicroROS {
 private:
  rcl_publisher_t publisher;
  std_msgs__msg__Int32 msg;

  rclc_executor_t executor;
  rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;
  rcl_timer_t timer;

  long previousErrorTime = 0;

  // Error handle loop
  void error_loop();
  // void timer_callback(rcl_timer_t* timer, int64_t last_call_time);

#define RCSOFTCHECK(fn)            \
  {                                \
    rcl_ret_t temp_rc = fn;        \
    if ((temp_rc != RCL_RET_OK)) { \
    }                              \
  }

#define RCCHECK(fn)                \
  {                                \
    rcl_ret_t temp_rc = fn;        \
    if ((temp_rc != RCL_RET_OK)) { \
      error_loop();                \
    }                              \
  }

 public:
  MicroROS();
  void init();
  void publish(int data);
};

#endif  // MOBILE_ROSS_HPP_
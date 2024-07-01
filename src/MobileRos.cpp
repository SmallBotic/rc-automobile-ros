#include "MobileRos.hpp"

// rcl_publisher_t publisher;
// std_msgs__msg__Int32 msg;
// rclc_executor_t executor;
// rclc_support_t support;
// rcl_allocator_t allocator;
// rcl_node_t node;
// rcl_timer_t timer;

// long previousErrorTime = 0;

void MicroROS::error_loop() {
  while (1) {
    if(millis() - previousErrorTime > 1000) {
      previousErrorTime = millis();
      DEBUG_PRINTLN("Error, stopping");
    }
  }
}

// void MicroROS::timer_callback(rcl_timer_t* timer, int64_t last_call_time) {
//   RCLC_UNUSED(last_call_time);
//   if (timer != NULL) {
//     DEBUG_PRINTLN("Timer callback publishing message");
//     RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
//     msg.data++;
//     DEBUG_PRINT("Published: ");
//     DEBUG_PRINTLN(msg.data);
//   }
// }

MicroROS::MicroROS() {
}

void MicroROS::init() {
  set_microros_wifi_transports(WIFI_SSID, WIFI_PASSWORD, ROS_AGENT_IP,
                               ROS_AGENT_PORT);
  delay(2000);

  allocator = rcl_get_default_allocator();

  // create init_options
  DEBUG_PRINTLN("Creating init_options");
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  DEBUG_PRINTLN("Init_options created");

  // create node
  DEBUG_PRINTLN("Creating node");
  RCCHECK(
      rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));
  DEBUG_PRINTLN("Node created");

  // create publisher
  DEBUG_PRINTLN("Creating publisher");
  RCCHECK(rclc_publisher_init_default(
      &publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "micro_ros_platformio_node_publisher"));
  DEBUG_PRINTLN("Publisher created");

  // create timer,
  // const unsigned int timer_timeout = 1000;
  // DEBUG_PRINTLN("Creating timer");
  // RCCHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(timer_timeout),
  //                                 timer_callback));
  // DEBUG_PRINTLN("Timer created");

  // create executor
  DEBUG_PRINTLN("Creating executor");
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  DEBUG_PRINTLN("Executor created");

  DEBUG_PRINTLN("Adding timer to executor");
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  DEBUG_PRINTLN("Timer added to executor");

  msg.data = 0;
}

void MicroROS::publish(int data) {
  msg.data = data;
  DEBUG_PRINT("Published: ");
  DEBUG_PRINTLN(msg.data);
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}

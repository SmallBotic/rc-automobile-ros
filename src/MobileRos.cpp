#include "MobileRos.hpp"

void MicroROS::error_loop() {
  while (1) {
    if (millis() - previousErrorTime > MICRO_ROS_ERROR_TIMEOUT) {
      previousErrorTime = millis();
      DEBUG_PRINTLN("\t\tError, stopping");
    }
  }
}

MicroROS::MicroROS()
    : previousErrorTime(0), allocator(rcl_get_default_allocator()) {}

MicroROS::~MicroROS() {
  RCCHECK(rcl_publisher_fini(&distancePublisher, &node));
  RCCHECK(rcl_publisher_fini(&imuPublisher, &node));
  RCCHECK(rcl_subscription_fini(&cmdVelSubscriber, &node));
  RCCHECK(rcl_node_fini(&node));
  RCCHECK(rclc_executor_fini(&executor));
}

void MicroROS::init() {
  set_microros_wifi_transports(WIFI_SSID, WIFI_PASSWORD, ROS_AGENT_IP,
                               ROS_AGENT_PORT);
  delay(2000);

  // Create init_options.
  rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
  RCCHECK(rcl_init_options_init(&init_options, allocator));

  // create init_options
  DEBUG_PRINTLN("Creating init_options");
  // RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options,
                                         &allocator));
  DEBUG_PRINTLN("Init options created");

  // create node
  DEBUG_PRINTLN("Creating node");
  node = rcl_get_zero_initialized_node();
  RCCHECK(rclc_node_init_default(&node, MICRO_ROS_NODE_NAME,
                                 MICRO_ROS_NODE_NAMESPACE, &support));
  DEBUG_PRINTLN("Node created");

  // create publishers
  DEBUG_PRINTLN("Creating publishers");
  RCCHECK(rclc_publisher_init_default(
      &distancePublisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      MICRO_ROS_DISTANCE_PUBLISH_TOPIC));
  RCCHECK(rclc_publisher_init_default(
      &imuPublisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      MICRO_ROS_IMU_PUBLISH_TOPIC));
  DEBUG_PRINTLN("Publishers created");

  // create subscriptions
  DEBUG_PRINTLN("Creating subscriptions");
  RCCHECK(rclc_subscription_init_default(
      // twist messg sub
      &cmdVelSubscriber, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      MICRO_ROS_CMD_VEL_SUBSCRIPTION_TOPIC));
  DEBUG_PRINTLN("Subscriptions created");

  // create executor
  DEBUG_PRINTLN("Creating executor");
  executor = rclc_executor_get_zero_initialized_executor();
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  DEBUG_PRINTLN("Executor created");

  DEBUG_PRINTLN("Adding timer to executor");
  unsigned int rcl_wait_timeout = 1000;  // in ms
  RCCHECK(rclc_timer_init_default(&timer, &support,
                                  RCL_MS_TO_NS(rcl_wait_timeout), NULL));
  // RCCHECK(rclc_executor_set_timeout(&executor,
  // RCL_MS_TO_NS(rcl_wait_timeout)));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  DEBUG_PRINTLN("Timer created and added to executor");

  DEBUG_PRINTLN("Adding subscription to executor");
  RCCHECK(rclc_executor_add_subscription(&executor, &cmdVelSubscriber,
                                         &controlMsg, &cmdVelSubCallback,
                                         ON_NEW_DATA));
  DEBUG_PRINTLN("Subscription added to executor");

  distanceMsg.data = 0;
}

void MicroROS::spin() {
  rcl_ret_t x = rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
  // DEBUG_PRINTF("Spin: %d\n", x);
  RCCHECK(x);
}

void MicroROS::publish(MicroROS::PublisherType type, int data) {
  switch (type) {
    case DISTANCE: {
      distanceMsg.data = data;
      rcl_ret_t x = rcl_publish(&distancePublisher, &distanceMsg, NULL);
      RCSOFTCHECK(x);
      // DEBUG_PRINTF("Published: (%d) %d\n", x, data);
      break;
    }
    default: {
      break;
    }
  }
}

void MicroROS::publish(MicroROS::PublisherType type, String data) {
  switch (type) {
    case IMU: {
      imuMsg = data;
      RCSOFTCHECK(rcl_publish(&imuPublisher, &imuMsg, NULL));
      break;
    }
    default: {
      break;
    }
  }
}

rcl_publisher_t* MicroROS::getPublisher(MicroROS::PublisherType type) {
  switch (type) {
    case DISTANCE: {
      return &distancePublisher;
    }
    case IMU: {
      return &imuPublisher;
    }
    default: {
      return NULL;
    }
  }
}

void MicroROS::cmdVelSubCallback(const void* msgIn) {
  const geometry_msgs__msg__Twist* msg =
      (const geometry_msgs__msg__Twist*)msgIn;

  DEBUG_PRINTF("Received: lX: %f, aZ: %f\n", msg->linear.x, msg->angular.z);
}

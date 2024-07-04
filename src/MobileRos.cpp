#include "MobileRos.hpp"

void MicroROS::error_loop() {
  while (1) {
    if (millis() - previousErrorTime > 1000) {
      previousErrorTime = millis();
      DEBUG_PRINTLN("Error, stopping");
    }
  }
}

MicroROS::MicroROS()
    : previousErrorTime(0), allocator(rcl_get_default_allocator()) {}

void MicroROS::init() {
  set_microros_wifi_transports(WIFI_SSID, WIFI_PASSWORD, ROS_AGENT_IP,
                               ROS_AGENT_PORT);
  delay(2000);

  // create init_options
  DEBUG_PRINTLN("Creating init_options");
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  DEBUG_PRINTLN("Init_options created");

  // create node
  DEBUG_PRINTLN("Creating node");
  RCCHECK(rclc_node_init_default(&node, MICRO_ROS_NODE_NAME, "", &support));
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
      &subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      MICRO_ROS_CMD_VEL_SUBSCRIPTION_TOPIC));
  DEBUG_PRINTLN("Subscriptions created");

  // create executor
  DEBUG_PRINTLN("Creating executor");
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  DEBUG_PRINTLN("Executor created");

  DEBUG_PRINTLN("Adding timer to executor");
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  DEBUG_PRINTLN("Timer added to executor");

  distanceMsg.data = 0;
}

void MicroROS::publish(MicroROS::PublisherType type, int data) {
  switch (type) {
    case DISTANCE:
      distanceMsg.data = data;
      RCSOFTCHECK(rcl_publish(&distancePublisher, &distanceMsg, NULL));
      break;
    default:
      break;
  }

  // distanceMsg.data = data;
  // RCSOFTCHECK(rcl_publish(&distancePublisher, &distanceMsg, NULL));
  DEBUG_PRINT("Published: ");
  DEBUG_PRINTLN(data);
}

void MicroROS::publish(MicroROS::PublisherType type, String data) {
  switch (type) {
    case IMU:
      imuMsg = data;
      RCSOFTCHECK(rcl_publish(&imuPublisher, &imuMsg, NULL));
      break;
    default:
      break;
  }
}

  int MicroROS::receiveSubscription() {
    std_msgs__msg__Int32 msg;
    rcl_ret_t ret = rcl_take(&subscriber, &msg, NULL, NULL);
    if (ret == RCL_RET_OK) {
      DEBUG_PRINT("Received: ");
      DEBUG_PRINTLN(msg.data);
      return msg.data;
    } else if (ret == RCL_RET_INVALID_ARGUMENT) {
      DEBUG_PRINTLN("Invalid argument");
    } else if (ret == RCL_RET_ERROR) {
      DEBUG_PRINTLN("Error");
    } else if (ret == RCL_RET_TIMEOUT) {
      DEBUG_PRINTLN("Timeout");
    } else if (ret == RCL_RET_BAD_ALLOC) {
      DEBUG_PRINTLN("Bad alloc");
    } else if (ret == RCL_RET_NODE_INVALID) {
      DEBUG_PRINTLN("Node invalid");
    } else if (ret == RCL_RET_NOT_INIT) {
      DEBUG_PRINTLN("Not init");
    } else {
      DEBUG_PRINTF("Unknown error: %d", ret);
    }
    return -1;
  }

  rcl_publisher_t* MicroROS::getPublisher(MicroROS::PublisherType type) {
    switch (type) {
      case DISTANCE:
        return &distancePublisher;
      default:
        return NULL;
    }
  }

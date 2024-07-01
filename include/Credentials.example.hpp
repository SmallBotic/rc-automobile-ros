#ifndef CREDENTIALS_HPP_
#define CREDENTIALS_HPP_

#include <IPAddress.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Change this to your ROS2 agent IP
// (the machine running the ROS2 agent e.g. laptop, raspberry pi, etc.)
#define ROS_AGENT_IP IPAddress(192, 168, 0, 3)
#define ROS_AGENT_PORT 8888 // Default port for ROS2 agent, change if necessary

#endif  // CREDENTIALS_HPP_
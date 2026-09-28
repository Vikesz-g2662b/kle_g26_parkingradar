cmake_minimum_required(VERSION 3.8)
project(kle_g26_parkingradar)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

set(ament_dependencies
  rclcpp
  std_msgs
  sensor_msgs
)

add_executable(distance_sensor src/distance_sensor.cpp)
ament_target_dependencies(distance_sensor ${ament_dependencies})

add_executable(parking_radar src/parking_radar.cpp)
ament_target_dependencies(parking_radar ${ament_dependencies})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
distance_sensor
parking_radar
DESTINATION lib/${PROJECT_NAME})

ament_package()

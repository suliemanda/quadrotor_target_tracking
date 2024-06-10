cmake_minimum_required(VERSION 3.8)
project(ground_robot_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_interfaces REQUIRED)
find_package(Eigen3 3.3 REQUIRED)
find_package(gazebo_msgs REQUIRED)


# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(ground_robot_PID src/ground_robot_PID.cpp)
ament_target_dependencies(
  ground_robot_PID
  "Eigen3"
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
  "custom_interfaces"
  "gazebo_msgs"
)
target_link_libraries (ground_robot_PID Eigen3::Eigen)


target_include_directories(ground_robot_PID PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(ground_robot_PID PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS ground_robot_PID
  DESTINATION lib/${PROJECT_NAME})

add_executable(ground_robot_reference src/ground_robot_reference.cpp)
ament_target_dependencies(
  ground_robot_reference
  "Eigen3"
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
  "custom_interfaces"
  "gazebo_msgs"
)
target_link_libraries (ground_robot_reference Eigen3::Eigen)
target_include_directories(ground_robot_reference PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(ground_robot_reference PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
install(TARGETS ground_robot_reference
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

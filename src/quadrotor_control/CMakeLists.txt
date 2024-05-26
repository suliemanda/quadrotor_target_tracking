cmake_minimum_required(VERSION 3.8)
project(quadrotor_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(custom_interfaces REQUIRED)
find_package(Eigen3 3.3 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(casadi REQUIRED)
find_package(gazebo_msgs REQUIRED)

add_executable(quadrotor_mpc src/quadrotor_mpc.cpp)
target_link_libraries (quadrotor_mpc Eigen3::Eigen yaml-cpp casadi)

target_include_directories(quadrotor_mpc PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
     $<INSTALL_INTERFACE:include>)
target_compile_features(quadrotor_mpc PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  quadrotor_mpc
  "Eigen3"
  "casadi"
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
  "custom_interfaces"
  "yaml-cpp"
  "gazebo_msgs"
)

install(TARGETS quadrotor_mpc
  DESTINATION lib/${PROJECT_NAME})



  add_executable(quadrotor_vision src/quadrotor_vision.cpp)
  target_link_libraries (quadrotor_vision Eigen3::Eigen yaml-cpp casadi)
  
  target_include_directories(quadrotor_vision PUBLIC
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
       $<INSTALL_INTERFACE:include>)
  target_compile_features(quadrotor_vision PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
  ament_target_dependencies(
    quadrotor_vision
    "Eigen3"
    "rclcpp"
    "geometry_msgs"
    "std_msgs"
    "custom_interfaces"
    "yaml-cpp"
    "gazebo_msgs"
  )
  
  install(TARGETS quadrotor_vision
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

// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_H_
#define CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'current_pose'
// Member 'future_pose'
#include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'current_twist'
// Member 'future_twist'
#include "geometry_msgs/msg/detail/twist__struct.h"

/// Struct defined in msg/ReferenceState in the package custom_interfaces.
/**
  * Message Type for the desired quadrotor state, supports receding horizon controllers by providing a list of future states
 */
typedef struct custom_interfaces__msg__ReferenceState
{
  std_msgs__msg__Header header;
  geometry_msgs__msg__Pose current_pose;
  geometry_msgs__msg__Twist current_twist;
  /// number of future states (for receding horizon controllers)
  uint32_t n;
  /// time between future states
  float dt;
  geometry_msgs__msg__Pose__Sequence future_pose;
  geometry_msgs__msg__Twist__Sequence future_twist;
} custom_interfaces__msg__ReferenceState;

// Struct for a sequence of custom_interfaces__msg__ReferenceState.
typedef struct custom_interfaces__msg__ReferenceState__Sequence
{
  custom_interfaces__msg__ReferenceState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} custom_interfaces__msg__ReferenceState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_H_

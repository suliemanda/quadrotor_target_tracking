// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "custom_interfaces/msg/detail/reference_state__rosidl_typesupport_introspection_c.h"
#include "custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "custom_interfaces/msg/detail/reference_state__functions.h"
#include "custom_interfaces/msg/detail/reference_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `current_pose`
// Member `future_pose`
#include "geometry_msgs/msg/pose.h"
// Member `current_pose`
// Member `future_pose`
#include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `current_twist`
// Member `future_twist`
#include "geometry_msgs/msg/twist.h"
// Member `current_twist`
// Member `future_twist`
#include "geometry_msgs/msg/detail/twist__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  custom_interfaces__msg__ReferenceState__init(message_memory);
}

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_fini_function(void * message_memory)
{
  custom_interfaces__msg__ReferenceState__fini(message_memory);
}

size_t custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__size_function__ReferenceState__future_pose(
  const void * untyped_member)
{
  const geometry_msgs__msg__Pose__Sequence * member =
    (const geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return member->size;
}

const void * custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_pose(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Pose__Sequence * member =
    (const geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return &member->data[index];
}

void * custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_pose(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Pose__Sequence * member =
    (geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  return &member->data[index];
}

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__fetch_function__ReferenceState__future_pose(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Pose * item =
    ((const geometry_msgs__msg__Pose *)
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_pose(untyped_member, index));
  geometry_msgs__msg__Pose * value =
    (geometry_msgs__msg__Pose *)(untyped_value);
  *value = *item;
}

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__assign_function__ReferenceState__future_pose(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Pose * item =
    ((geometry_msgs__msg__Pose *)
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_pose(untyped_member, index));
  const geometry_msgs__msg__Pose * value =
    (const geometry_msgs__msg__Pose *)(untyped_value);
  *item = *value;
}

bool custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__resize_function__ReferenceState__future_pose(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Pose__Sequence * member =
    (geometry_msgs__msg__Pose__Sequence *)(untyped_member);
  geometry_msgs__msg__Pose__Sequence__fini(member);
  return geometry_msgs__msg__Pose__Sequence__init(member, size);
}

size_t custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__size_function__ReferenceState__future_twist(
  const void * untyped_member)
{
  const geometry_msgs__msg__Twist__Sequence * member =
    (const geometry_msgs__msg__Twist__Sequence *)(untyped_member);
  return member->size;
}

const void * custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_twist(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__Twist__Sequence * member =
    (const geometry_msgs__msg__Twist__Sequence *)(untyped_member);
  return &member->data[index];
}

void * custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_twist(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__Twist__Sequence * member =
    (geometry_msgs__msg__Twist__Sequence *)(untyped_member);
  return &member->data[index];
}

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__fetch_function__ReferenceState__future_twist(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const geometry_msgs__msg__Twist * item =
    ((const geometry_msgs__msg__Twist *)
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_twist(untyped_member, index));
  geometry_msgs__msg__Twist * value =
    (geometry_msgs__msg__Twist *)(untyped_value);
  *value = *item;
}

void custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__assign_function__ReferenceState__future_twist(
  void * untyped_member, size_t index, const void * untyped_value)
{
  geometry_msgs__msg__Twist * item =
    ((geometry_msgs__msg__Twist *)
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_twist(untyped_member, index));
  const geometry_msgs__msg__Twist * value =
    (const geometry_msgs__msg__Twist *)(untyped_value);
  *item = *value;
}

bool custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__resize_function__ReferenceState__future_twist(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__Twist__Sequence * member =
    (geometry_msgs__msg__Twist__Sequence *)(untyped_member);
  geometry_msgs__msg__Twist__Sequence__fini(member);
  return geometry_msgs__msg__Twist__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[7] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, current_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_twist",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, current_twist),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "n",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, n),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "dt",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, dt),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "future_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, future_pose),  // bytes offset in struct
    NULL,  // default value
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__size_function__ReferenceState__future_pose,  // size() function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_pose,  // get_const(index) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_pose,  // get(index) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__fetch_function__ReferenceState__future_pose,  // fetch(index, &value) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__assign_function__ReferenceState__future_pose,  // assign(index, value) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__resize_function__ReferenceState__future_pose  // resize(index) function pointer
  },
  {
    "future_twist",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(custom_interfaces__msg__ReferenceState, future_twist),  // bytes offset in struct
    NULL,  // default value
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__size_function__ReferenceState__future_twist,  // size() function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_const_function__ReferenceState__future_twist,  // get_const(index) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__get_function__ReferenceState__future_twist,  // get(index) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__fetch_function__ReferenceState__future_twist,  // fetch(index, &value) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__assign_function__ReferenceState__future_twist,  // assign(index, value) function pointer
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__resize_function__ReferenceState__future_twist  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_members = {
  "custom_interfaces__msg",  // message namespace
  "ReferenceState",  // message name
  7,  // number of fields
  sizeof(custom_interfaces__msg__ReferenceState),
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array,  // message members
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_init_function,  // function to initialize message memory (memory has to be allocated)
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_type_support_handle = {
  0,
  &custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, custom_interfaces, msg, ReferenceState)() {
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Twist)();
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[5].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_member_array[6].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Twist)();
  if (!custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_type_support_handle.typesupport_identifier) {
    custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &custom_interfaces__msg__ReferenceState__rosidl_typesupport_introspection_c__ReferenceState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

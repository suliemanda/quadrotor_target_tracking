// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice
#include "custom_interfaces/msg/detail/reference_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `current_pose`
// Member `future_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `current_twist`
// Member `future_twist`
#include "geometry_msgs/msg/detail/twist__functions.h"

bool
custom_interfaces__msg__ReferenceState__init(custom_interfaces__msg__ReferenceState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    custom_interfaces__msg__ReferenceState__fini(msg);
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__init(&msg->current_pose)) {
    custom_interfaces__msg__ReferenceState__fini(msg);
    return false;
  }
  // current_twist
  if (!geometry_msgs__msg__Twist__init(&msg->current_twist)) {
    custom_interfaces__msg__ReferenceState__fini(msg);
    return false;
  }
  // n
  // dt
  // future_pose
  if (!geometry_msgs__msg__Pose__Sequence__init(&msg->future_pose, 0)) {
    custom_interfaces__msg__ReferenceState__fini(msg);
    return false;
  }
  // future_twist
  if (!geometry_msgs__msg__Twist__Sequence__init(&msg->future_twist, 0)) {
    custom_interfaces__msg__ReferenceState__fini(msg);
    return false;
  }
  return true;
}

void
custom_interfaces__msg__ReferenceState__fini(custom_interfaces__msg__ReferenceState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // current_pose
  geometry_msgs__msg__Pose__fini(&msg->current_pose);
  // current_twist
  geometry_msgs__msg__Twist__fini(&msg->current_twist);
  // n
  // dt
  // future_pose
  geometry_msgs__msg__Pose__Sequence__fini(&msg->future_pose);
  // future_twist
  geometry_msgs__msg__Twist__Sequence__fini(&msg->future_twist);
}

bool
custom_interfaces__msg__ReferenceState__are_equal(const custom_interfaces__msg__ReferenceState * lhs, const custom_interfaces__msg__ReferenceState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->current_pose), &(rhs->current_pose)))
  {
    return false;
  }
  // current_twist
  if (!geometry_msgs__msg__Twist__are_equal(
      &(lhs->current_twist), &(rhs->current_twist)))
  {
    return false;
  }
  // n
  if (lhs->n != rhs->n) {
    return false;
  }
  // dt
  if (lhs->dt != rhs->dt) {
    return false;
  }
  // future_pose
  if (!geometry_msgs__msg__Pose__Sequence__are_equal(
      &(lhs->future_pose), &(rhs->future_pose)))
  {
    return false;
  }
  // future_twist
  if (!geometry_msgs__msg__Twist__Sequence__are_equal(
      &(lhs->future_twist), &(rhs->future_twist)))
  {
    return false;
  }
  return true;
}

bool
custom_interfaces__msg__ReferenceState__copy(
  const custom_interfaces__msg__ReferenceState * input,
  custom_interfaces__msg__ReferenceState * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->current_pose), &(output->current_pose)))
  {
    return false;
  }
  // current_twist
  if (!geometry_msgs__msg__Twist__copy(
      &(input->current_twist), &(output->current_twist)))
  {
    return false;
  }
  // n
  output->n = input->n;
  // dt
  output->dt = input->dt;
  // future_pose
  if (!geometry_msgs__msg__Pose__Sequence__copy(
      &(input->future_pose), &(output->future_pose)))
  {
    return false;
  }
  // future_twist
  if (!geometry_msgs__msg__Twist__Sequence__copy(
      &(input->future_twist), &(output->future_twist)))
  {
    return false;
  }
  return true;
}

custom_interfaces__msg__ReferenceState *
custom_interfaces__msg__ReferenceState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__ReferenceState * msg = (custom_interfaces__msg__ReferenceState *)allocator.allocate(sizeof(custom_interfaces__msg__ReferenceState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(custom_interfaces__msg__ReferenceState));
  bool success = custom_interfaces__msg__ReferenceState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
custom_interfaces__msg__ReferenceState__destroy(custom_interfaces__msg__ReferenceState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    custom_interfaces__msg__ReferenceState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
custom_interfaces__msg__ReferenceState__Sequence__init(custom_interfaces__msg__ReferenceState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__ReferenceState * data = NULL;

  if (size) {
    data = (custom_interfaces__msg__ReferenceState *)allocator.zero_allocate(size, sizeof(custom_interfaces__msg__ReferenceState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = custom_interfaces__msg__ReferenceState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        custom_interfaces__msg__ReferenceState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
custom_interfaces__msg__ReferenceState__Sequence__fini(custom_interfaces__msg__ReferenceState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      custom_interfaces__msg__ReferenceState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

custom_interfaces__msg__ReferenceState__Sequence *
custom_interfaces__msg__ReferenceState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__ReferenceState__Sequence * array = (custom_interfaces__msg__ReferenceState__Sequence *)allocator.allocate(sizeof(custom_interfaces__msg__ReferenceState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = custom_interfaces__msg__ReferenceState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
custom_interfaces__msg__ReferenceState__Sequence__destroy(custom_interfaces__msg__ReferenceState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    custom_interfaces__msg__ReferenceState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
custom_interfaces__msg__ReferenceState__Sequence__are_equal(const custom_interfaces__msg__ReferenceState__Sequence * lhs, const custom_interfaces__msg__ReferenceState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!custom_interfaces__msg__ReferenceState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
custom_interfaces__msg__ReferenceState__Sequence__copy(
  const custom_interfaces__msg__ReferenceState__Sequence * input,
  custom_interfaces__msg__ReferenceState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(custom_interfaces__msg__ReferenceState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    custom_interfaces__msg__ReferenceState * data =
      (custom_interfaces__msg__ReferenceState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!custom_interfaces__msg__ReferenceState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          custom_interfaces__msg__ReferenceState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!custom_interfaces__msg__ReferenceState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}

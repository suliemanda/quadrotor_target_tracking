// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'current_pose'
// Member 'future_pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"
// Member 'current_twist'
// Member 'future_twist'
#include "geometry_msgs/msg/detail/twist__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__custom_interfaces__msg__ReferenceState __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__msg__ReferenceState __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ReferenceState_
{
  using Type = ReferenceState_<ContainerAllocator>;

  explicit ReferenceState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    current_pose(_init),
    current_twist(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->n = 0ul;
      this->dt = 0.0f;
    }
  }

  explicit ReferenceState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    current_pose(_alloc, _init),
    current_twist(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->n = 0ul;
      this->dt = 0.0f;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _current_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _current_pose_type current_pose;
  using _current_twist_type =
    geometry_msgs::msg::Twist_<ContainerAllocator>;
  _current_twist_type current_twist;
  using _n_type =
    uint32_t;
  _n_type n;
  using _dt_type =
    float;
  _dt_type dt;
  using _future_pose_type =
    std::vector<geometry_msgs::msg::Pose_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Pose_<ContainerAllocator>>>;
  _future_pose_type future_pose;
  using _future_twist_type =
    std::vector<geometry_msgs::msg::Twist_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Twist_<ContainerAllocator>>>;
  _future_twist_type future_twist;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__current_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->current_pose = _arg;
    return *this;
  }
  Type & set__current_twist(
    const geometry_msgs::msg::Twist_<ContainerAllocator> & _arg)
  {
    this->current_twist = _arg;
    return *this;
  }
  Type & set__n(
    const uint32_t & _arg)
  {
    this->n = _arg;
    return *this;
  }
  Type & set__dt(
    const float & _arg)
  {
    this->dt = _arg;
    return *this;
  }
  Type & set__future_pose(
    const std::vector<geometry_msgs::msg::Pose_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Pose_<ContainerAllocator>>> & _arg)
  {
    this->future_pose = _arg;
    return *this;
  }
  Type & set__future_twist(
    const std::vector<geometry_msgs::msg::Twist_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<geometry_msgs::msg::Twist_<ContainerAllocator>>> & _arg)
  {
    this->future_twist = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::msg::ReferenceState_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::msg::ReferenceState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::ReferenceState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::ReferenceState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__msg__ReferenceState
    std::shared_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__msg__ReferenceState
    std::shared_ptr<custom_interfaces::msg::ReferenceState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ReferenceState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->current_pose != other.current_pose) {
      return false;
    }
    if (this->current_twist != other.current_twist) {
      return false;
    }
    if (this->n != other.n) {
      return false;
    }
    if (this->dt != other.dt) {
      return false;
    }
    if (this->future_pose != other.future_pose) {
      return false;
    }
    if (this->future_twist != other.future_twist) {
      return false;
    }
    return true;
  }
  bool operator!=(const ReferenceState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ReferenceState_

// alias to use template instance with default allocator
using ReferenceState =
  custom_interfaces::msg::ReferenceState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__STRUCT_HPP_

// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/reference_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_ReferenceState_future_twist
{
public:
  explicit Init_ReferenceState_future_twist(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::msg::ReferenceState future_twist(::custom_interfaces::msg::ReferenceState::_future_twist_type arg)
  {
    msg_.future_twist = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_future_pose
{
public:
  explicit Init_ReferenceState_future_pose(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  Init_ReferenceState_future_twist future_pose(::custom_interfaces::msg::ReferenceState::_future_pose_type arg)
  {
    msg_.future_pose = std::move(arg);
    return Init_ReferenceState_future_twist(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_dt
{
public:
  explicit Init_ReferenceState_dt(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  Init_ReferenceState_future_pose dt(::custom_interfaces::msg::ReferenceState::_dt_type arg)
  {
    msg_.dt = std::move(arg);
    return Init_ReferenceState_future_pose(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_n
{
public:
  explicit Init_ReferenceState_n(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  Init_ReferenceState_dt n(::custom_interfaces::msg::ReferenceState::_n_type arg)
  {
    msg_.n = std::move(arg);
    return Init_ReferenceState_dt(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_current_twist
{
public:
  explicit Init_ReferenceState_current_twist(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  Init_ReferenceState_n current_twist(::custom_interfaces::msg::ReferenceState::_current_twist_type arg)
  {
    msg_.current_twist = std::move(arg);
    return Init_ReferenceState_n(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_current_pose
{
public:
  explicit Init_ReferenceState_current_pose(::custom_interfaces::msg::ReferenceState & msg)
  : msg_(msg)
  {}
  Init_ReferenceState_current_twist current_pose(::custom_interfaces::msg::ReferenceState::_current_pose_type arg)
  {
    msg_.current_pose = std::move(arg);
    return Init_ReferenceState_current_twist(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

class Init_ReferenceState_header
{
public:
  Init_ReferenceState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ReferenceState_current_pose header(::custom_interfaces::msg::ReferenceState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ReferenceState_current_pose(msg_);
  }

private:
  ::custom_interfaces::msg::ReferenceState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::ReferenceState>()
{
  return custom_interfaces::msg::builder::Init_ReferenceState_header();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__BUILDER_HPP_

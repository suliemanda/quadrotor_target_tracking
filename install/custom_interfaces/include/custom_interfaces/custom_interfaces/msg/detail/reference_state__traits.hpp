// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from custom_interfaces:msg/ReferenceState.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__TRAITS_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "custom_interfaces/msg/detail/reference_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'current_pose'
// Member 'future_pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"
// Member 'current_twist'
// Member 'future_twist'
#include "geometry_msgs/msg/detail/twist__traits.hpp"

namespace custom_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const ReferenceState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: current_pose
  {
    out << "current_pose: ";
    to_flow_style_yaml(msg.current_pose, out);
    out << ", ";
  }

  // member: current_twist
  {
    out << "current_twist: ";
    to_flow_style_yaml(msg.current_twist, out);
    out << ", ";
  }

  // member: n
  {
    out << "n: ";
    rosidl_generator_traits::value_to_yaml(msg.n, out);
    out << ", ";
  }

  // member: dt
  {
    out << "dt: ";
    rosidl_generator_traits::value_to_yaml(msg.dt, out);
    out << ", ";
  }

  // member: future_pose
  {
    if (msg.future_pose.size() == 0) {
      out << "future_pose: []";
    } else {
      out << "future_pose: [";
      size_t pending_items = msg.future_pose.size();
      for (auto item : msg.future_pose) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: future_twist
  {
    if (msg.future_twist.size() == 0) {
      out << "future_twist: []";
    } else {
      out << "future_twist: [";
      size_t pending_items = msg.future_twist.size();
      for (auto item : msg.future_twist) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ReferenceState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: current_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_pose:\n";
    to_block_style_yaml(msg.current_pose, out, indentation + 2);
  }

  // member: current_twist
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_twist:\n";
    to_block_style_yaml(msg.current_twist, out, indentation + 2);
  }

  // member: n
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "n: ";
    rosidl_generator_traits::value_to_yaml(msg.n, out);
    out << "\n";
  }

  // member: dt
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "dt: ";
    rosidl_generator_traits::value_to_yaml(msg.dt, out);
    out << "\n";
  }

  // member: future_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.future_pose.size() == 0) {
      out << "future_pose: []\n";
    } else {
      out << "future_pose:\n";
      for (auto item : msg.future_pose) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: future_twist
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.future_twist.size() == 0) {
      out << "future_twist: []\n";
    } else {
      out << "future_twist:\n";
      for (auto item : msg.future_twist) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ReferenceState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use custom_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const custom_interfaces::msg::ReferenceState & msg,
  std::ostream & out, size_t indentation = 0)
{
  custom_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use custom_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const custom_interfaces::msg::ReferenceState & msg)
{
  return custom_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<custom_interfaces::msg::ReferenceState>()
{
  return "custom_interfaces::msg::ReferenceState";
}

template<>
inline const char * name<custom_interfaces::msg::ReferenceState>()
{
  return "custom_interfaces/msg/ReferenceState";
}

template<>
struct has_fixed_size<custom_interfaces::msg::ReferenceState>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<custom_interfaces::msg::ReferenceState>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<custom_interfaces::msg::ReferenceState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__REFERENCE_STATE__TRAITS_HPP_

#include <rclcpp/rclcpp.hpp>
#include <custom_interfaces/msg/reference_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include "Eigen/Dense"
#include "gazebo_msgs/msg/entity_state.hpp"
#include "gazebo_msgs/srv/get_entity_state.hpp"
#include <chrono>
using namespace std::chrono_literals;

class GroundRobotCotroller : public rclcpp::Node
{
public:
  GroundRobotCotroller()
      : Node("ground_robot_cotroller")
  {
    this->declare_parameter("reference_topic", "/ground_robot_reference");
    this->declare_parameter("robot_name", "ground_robot");
    this->declare_parameter("state_service", "/gazebo_state_serv/get_entity_state");
    this->declare_parameter("cmd_vel_topic", "/ground_robot/cmd_vel");

    this->declare_parameter("K_t", 2.0);
    this->declare_parameter("K_n", 1.0);
    this->declare_parameter("K_phi", 0.400);

    robot_name = this->get_parameter("robot_name").as_string();
    state_service = this->get_parameter("state_service").as_string();
    ref_topic = this->get_parameter("reference_topic").as_string();
    cmd_vel_topic = this->get_parameter("cmd_vel_topic").as_string();
    K_t = this->get_parameter("K_t").as_double();
    K_n = this->get_parameter("K_n").as_double();
    K_phi = this->get_parameter("K_phi").as_double();
    auto timer_callback = [this]() -> void
    {
      // Code to be executed every 5ms
      // Call service to get entity state
      while (!state_client->wait_for_service(10ms))
      {
        if (!rclcpp::ok())
        {
          RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for service to appear.");
          return;
        }
        RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
      }

      auto request = std::make_shared<gazebo_msgs::srv::GetEntityState::Request>();
      request->name = robot_name;
      auto future = state_client->async_send_request(request);
      std::future_status status = future.wait_for(10ms);

      if (status == std::future_status::ready)
      {
        auto response = future.get();
        double x = response->state.pose.position.x;
        double y = response->state.pose.position.y;
        p = Eigen::Vector2d{x, y};
        double q_w = response->state.pose.orientation.w;
        double q_z = response->state.pose.orientation.z;
        phi = 2 * atan2(q_z, q_w);
      }
      else
      {
        RCLCPP_ERROR(this->get_logger(), "Failed to call service /gazebo_state_serv/get_entity_state");
      }
    };
    auto ref_callback = [this](custom_interfaces::msg::ReferenceState msg) -> void
    {
      // Code to be executed every time a new reference is received
      double x_ref = msg.current_pose.position.x;
      double y_ref = msg.current_pose.position.y;
      double phi_ref = 2 * atan2(msg.current_pose.orientation.z, msg.current_pose.orientation.w);
      double vx_ref = msg.current_twist.linear.x;
      double vy_ref = msg.current_twist.linear.y;
      double w_ref = msg.current_twist.angular.z;
      double e_x = x_ref - p(0);
      double e_y = y_ref - p(1);
      double e_t = cos(phi) * e_x + sin(phi) * e_y;
      double e_n = -sin(phi) * e_x + cos(phi) * e_y;
      double e_phi = phi_ref - phi;
      double vd = pow(pow(vx_ref, 2) + pow(vy_ref, 2), 0.5) * cos(e_phi) + K_t * e_t;
      double wd = w_ref + vd * (K_n * e_n + K_phi * sin(e_phi));
      custom_interfaces::msg::ReferenceState error_msg;
      error_msg.header.stamp = this->now();
      error_msg.current_pose.position.x = e_x;
      error_msg.current_pose.position.y = e_y;
      error_msg.current_pose.orientation.z = sin(e_phi / 2);
      error_msg.current_pose.orientation.w = cos(e_phi / 2);

      // Publish control commands
      geometry_msgs::msg::Twist cmd;
      cmd.linear.x = std::min(std::max(0.0, vd), 10.0);
      cmd.angular.z = std::min(std::max(-2.0, wd), 2.0);
      publisher_cmd->publish(cmd);
      error_pub->publish(error_msg);
    };
    g1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    g2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    timer = this->create_wall_timer(5ms, timer_callback, g1);
    state_client = this->create_client<gazebo_msgs::srv::GetEntityState>(state_service, rmw_qos_profile_services_default, g2);
    ref_sub = this->create_subscription<custom_interfaces::msg::ReferenceState>(ref_topic, 10, ref_callback);
    publisher_cmd = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, 10);
    error_pub= this->create_publisher<custom_interfaces::msg::ReferenceState>("/ground_robot_error", 10);
  }

private:
  // rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr publisher_ref;
  rclcpp::Subscription<custom_interfaces::msg::ReferenceState>::SharedPtr ref_sub;
  rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr error_pub;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_cmd;
  rclcpp::Client<gazebo_msgs::srv::GetEntityState>::SharedPtr state_client;
  rclcpp::CallbackGroup::SharedPtr g1;
  rclcpp::CallbackGroup::SharedPtr g2;
  rclcpp::TimerBase::SharedPtr timer;

  Eigen::Vector2d p{0, 0};
  double phi = 0;
  double t = 0;
  double I_x = 0;
  double I_y = 0;
  double K_t;
  double K_n;
  double K_phi;
  std::string state_service;
  std::string ref_topic;
  std::string robot_name;
  std::string cmd_vel_topic;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor exec;
  auto node = std::make_shared<GroundRobotCotroller>();
  exec.add_node(node);
  exec.spin();
  RCLCPP_INFO(node->get_logger(), "Keyboard interrupt, shutting down.\n");

  // cont.take_off();
  // rclcpp::spin();
  rclcpp::shutdown();
}

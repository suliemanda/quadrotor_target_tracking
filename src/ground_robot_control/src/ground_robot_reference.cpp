#include <rclcpp/rclcpp.hpp>
#include <custom_interfaces/msg/reference_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <vector>
#include <cmath>

class GroundRobotReferencePublisher : public rclcpp::Node
{
public:
    GroundRobotReferencePublisher()
        : Node("ground_robot_reference_publisher")
    {
        this->declare_parameter("publish_rate", 100.0);
        this->declare_parameter("reference_topic", "/ground_robot_reference");

        publish_rate = this->get_parameter("publish_rate").as_double();
        ref_topic = this->get_parameter("reference_topic").as_string();

        // Control the publishing rate
        // Hz
        DT = 1.0 / this->publish_rate; // seconds

        RCLCPP_INFO(this->get_logger(), "Reference publisher node initialized");
        t_0 = this->now().seconds();
        auto timer_callback = [this]() -> void
        {
            t = this->now().seconds() - t_0;
            if (t >= 10.0)
            {
                // double a = 10.0; // adjust the value of 'a' to control the shape of the Lemniscate
                // double t_tr = 2.0 * M_PI *(t-5.0) / 200.0;
                // double x_r = a *( cos(t_tr) / (1 + pow(sin(t_tr), 2))-1);
                // double y_r = a * sin(t_tr) * cos(t_tr) / (1 + pow(sin(t_tr), 2));
                // double x_dot_r = -a * sin(t_tr) * (1 + pow(sin(t_tr), 2) - 2 * pow(sin(t_tr), 2)) / pow(1 + pow(sin(t_tr), 2), 2);
                // double y_dot_r = a * (cos(t_tr) * cos(t_tr) - sin(t_tr) * sin(t_tr)) / (1 + pow(sin(t_tr), 2)) - a * sin(t_tr) * cos(t_tr) * (1 + pow(sin(t_tr), 2) - 2 * pow(sin(t_tr), 2)) / pow(1 + pow(sin(t_tr), 2), 2);
                // double phi_r = atan2(y_dot_r, x_dot_r);

                // circular_trajectory
                double radius = 5.0;
                double center_x = 0.0;
                double center_y = -5.0;
                double angular_velocity = 0.4; // Adjust the angular velocity as needed

                double x_r = center_x + radius * sin(angular_velocity * (t-10));
                double y_r = center_y + radius * cos(angular_velocity * (t-10));
                double x_dot_r = radius * angular_velocity * cos(angular_velocity * (t-10));
                double y_dot_r = -radius * angular_velocity * sin(angular_velocity * (t-10));

                double phi_r = atan2(radius * angular_velocity * cos(angular_velocity * (t-10)), -radius * angular_velocity * sin(angular_velocity * (t-10)));

                // double x_r=1.0;
                // double y_r=1.0;
                // double phi_r=M_PI_4;

                custom_interfaces::msg::ReferenceState reference_state_msg;
                reference_state_msg.header.stamp = this->now();
                reference_state_msg.current_pose.position.x = x_r;
                reference_state_msg.current_pose.position.y = y_r;
                reference_state_msg.current_pose.orientation.z = sin(phi_r / 2);
                reference_state_msg.current_pose.orientation.w = cos(phi_r / 2);
                reference_state_msg.current_twist.linear.x = x_dot_r;
                reference_state_msg.current_twist.linear.y = y_dot_r;
                reference_state_msg.current_twist.angular.z = angular_velocity;

                publisher_ref->publish(reference_state_msg);
            }
        };

        publisher_ref = this->create_publisher<custom_interfaces::msg::ReferenceState>(
            ref_topic,
            10);
        timer = this->create_wall_timer(std::chrono::duration<double>(this->DT),
                                        timer_callback);
    }

private:
    rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr publisher_ref;
    double publish_rate;
    std::string ref_topic;
    double DT;
    rclcpp::TimerBase::SharedPtr timer;
    double t = 0;
    double t_0 = 0;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GroundRobotReferencePublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

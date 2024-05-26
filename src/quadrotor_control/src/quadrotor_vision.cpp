#include <rclcpp/rclcpp.hpp>
#include <custom_interfaces/msg/reference_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <vector>
#include <cmath>

class QuadrotorReferencePublisher : public rclcpp::Node
{
public:
    QuadrotorReferencePublisher()
        : Node("quadrotor_reference_publisher")
    {
        this->declare_parameter("dt", 0.1);
        this->declare_parameter("N", 10);
        this->declare_parameter("publish_rate", 50.0);
        this->declare_parameter("reference_topic", "quadrotor_reference");

        n_future = this->get_parameter("N").as_int();

        dt_future = this->get_parameter("dt").as_double();
        publish_rate = this->get_parameter("publish_rate").as_double();
        ref_topic = this->get_parameter("reference_topic").as_string();

       

        // Control the publishing rate
        // Hz
        DT = 1.0 / this->publish_rate; // seconds
       

        RCLCPP_INFO(this->get_logger(), "Reference publisher node initialized");
        auto timer_callback = [this]() -> void
        {
            custom_interfaces::msg::ReferenceState reference_state_msg;
            reference_state_msg.header.stamp = this->now();
            reference_state_msg.current_pose.position.x = 5.0;
            reference_state_msg.current_pose.position.y = 0.0;
            reference_state_msg.current_pose.position.z = 1.0;
            reference_state_msg.current_pose.orientation.x = 0.0;
            reference_state_msg.current_pose.orientation.y = 0.0;
            reference_state_msg.current_pose.orientation.z = sin(M_PI/6);
            reference_state_msg.current_pose.orientation.w = cos(M_PI/6);
            reference_state_msg.current_twist.linear.x = 0.0;
            reference_state_msg.current_twist.linear.y = 0.0;
            reference_state_msg.current_twist.linear.z = 0.0;
            reference_state_msg.current_twist.angular.x = 0.0;
            reference_state_msg.current_twist.angular.y = 0.0;
            reference_state_msg.current_twist.angular.z = 0.0;
            for (int i = 0; i < n_future; i++)
            {
                reference_state_msg.future_pose.push_back(reference_state_msg.current_pose);
                reference_state_msg.future_twist.push_back(reference_state_msg.current_twist);
            }
            publisher_ref->publish(reference_state_msg);
        };
         publisher_ref = this->create_publisher<custom_interfaces::msg::ReferenceState>(
            ref_topic,
            10 // Queue size
        );
         timer = this->create_wall_timer(std::chrono::duration<double>(this->DT),
                                        timer_callback);
    }

private:


    rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr publisher_ref;

    int n_future;
    double dt_future;
    double publish_rate;
    std::string ref_topic;
    double DT;
    rclcpp::TimerBase::SharedPtr timer;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<QuadrotorReferencePublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

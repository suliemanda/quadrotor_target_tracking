#include <memory>
#include <chrono>
#include <string>
#include <fstream>
#include <unsupported/Eigen/MatrixFunctions>
#include "Eigen/Dense"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/wrench.hpp"
#include "custom_interfaces/msg/reference_state.hpp"
#include "gazebo_msgs/msg/entity_state.hpp"
#include "gazebo_msgs/srv/get_entity_state.hpp"
#include <yaml-cpp/yaml.h>
#include "std_msgs/msg/header.hpp"
#include "builtin_interfaces/msg/time.hpp"
#include "quadrotor_MPC.h"

using namespace std::chrono_literals;

class quadrotor_mpc : public rclcpp::Node
{

public:
    quadrotor_mpc() : Node("quadrotor_mpc")
    {
        this->declare_parameter("N", 10);
        this->declare_parameter("Q", std::vector<double>{120.0, 120.0, 120.0, 10.0, 10.0, 10.0, 20.0, 20.0, 2.0, 5.0, 5.0, 0.2});
        this->declare_parameter("R", std::vector<double>{0.0, 0.0, 0.0});
        this->declare_parameter("dt", 0.1);
        this->declare_parameter("quadrotor_description", "dji_m100");
        this->declare_parameter("state_service", "/gazebo_state_serv/get_entity_state");
        this->declare_parameter("state_topic", "quadrotor_state");
        this->declare_parameter("reference_topic", "quadrotor_reference");
        this->declare_parameter("rotor_topic_pref", "/quadrotor/rotor");
        N = this->get_parameter("N").as_int();
        dt = this->get_parameter("dt").as_double();
        Q_cost = this->get_parameter("Q").as_double_array();
        R_cost = this->get_parameter("R").as_double_array();
        quadrotor_name = this->get_parameter("quadrotor_description").as_string();
        state_service = this->get_parameter("state_service").as_string();
        rotor_topic = this->get_parameter("rotor_topic_pref").as_string();
        ref_topic = this->get_parameter("reference_topic").as_string();
        state_topic = this->get_parameter("state_topic").as_string();
        get_params();

        std::vector<double> torque2rotors_v = {torque2rotors(0, 0), torque2rotors(1, 0), torque2rotors(2, 0), torque2rotors(3, 0),
                                               torque2rotors(0, 1), torque2rotors(1, 1), torque2rotors(2, 1), torque2rotors(3, 1),
                                               torque2rotors(0, 2), torque2rotors(1, 2), torque2rotors(2, 2), torque2rotors(3, 2),
                                               torque2rotors(0, 3), torque2rotors(1, 3), torque2rotors(2, 3), torque2rotors(3, 3)};
        mpc = MPC(M, J(0, 0), J(1, 1), J(2, 2), torque2rotors_v, t2w, dt, N);
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
            request->name = quadrotor_name;
            auto future = state_client->async_send_request(request);
            std::future_status status = future.wait_for(10ms);

            if (status == std::future_status::ready)
            {
                auto response = future.get();
                double x = response->state.pose.position.x;
                double y = response->state.pose.position.y;
                double z = response->state.pose.position.z;
                p = Eigen::Vector3d{x, y, z};
                // std::cout << "p:\n"
                //         << p << "\n";
                x = response->state.twist.linear.x;
                y = response->state.twist.linear.y;
                z = response->state.twist.linear.z;
                v = Eigen::Vector3d{x, y, z};
                x = response->state.twist.angular.x;
                y = response->state.twist.angular.y;
                z = response->state.twist.angular.z;
                Eigen::Vector3d w_world = Eigen::Vector3d{x, y, z};
                double q_w = response->state.pose.orientation.w;
                x = response->state.pose.orientation.x;
                y = response->state.pose.orientation.y;
                z = response->state.pose.orientation.z;
                Q = Eigen::Quaterniond(q_w, x, y, z);
                // Transform w to body frame
                Eigen::Matrix3d R = Q.toRotationMatrix();
                w = R.transpose() * w_world;
                pvec = {p.x(), p.y(), p.z()};
                vvec = {v.x(), v.y(), v.z()};
                wvec = {w.x(), w.y(), w.z()};
                if (Q.w() > 0)
                    qvec = {Q.w(), Q.x(), Q.y(), Q.z()};
                else
                    qvec = {-Q.w(), -Q.x(), -Q.y(), -Q.z()};
                custom_interfaces::msg::ReferenceState state_msg;
                state_msg.current_pose.position.x = p.x();
                state_msg.current_pose.position.y = p.y();
                state_msg.current_pose.position.z = p.z();
                state_msg.current_twist.linear.x = v.x();
                state_msg.current_twist.linear.y = v.y();
                state_msg.current_twist.linear.z = v.z();
                state_msg.current_twist.angular.x = w.x();
                state_msg.current_twist.angular.y = w.y();
                state_msg.current_twist.angular.z = w.z();
                state_msg.current_pose.orientation.w = Q.w();
                state_msg.current_pose.orientation.x = Q.x();
                state_msg.current_pose.orientation.y = Q.y();
                state_msg.current_pose.orientation.z = Q.z();

                quadrotor_state_publisher_->publish(state_msg);
            }
            else
            {
                RCLCPP_ERROR(this->get_logger(), "Failed to call service /gazebo_state_serv/get_entity_state");
            }
        };

        auto ref_callback = [this](custom_interfaces::msg::ReferenceState msg) -> void
        {
            auto start = std::chrono::high_resolution_clock::now();

            // Q_cost = {120.0, 120.0, 120.0, 10.0, 10.0, 10.0, 20.0, 20.0, 2.0, 5.0, 5.0, 0.2};
            std::vector<Point> trajd;
            std::vector<Point> trajvd;
            std::vector<double> trajwzd;
            std::vector<Eigen::Quaterniond> trajq;
            double xd, yd, zd, vxd, vyd, vzd, wzd;
            Eigen::Quaterniond qd;
            xd = msg.current_pose.position.x;
            yd = msg.current_pose.position.y;
            zd = msg.current_pose.position.z;

            vxd = msg.current_twist.linear.x;
            vyd = msg.current_twist.linear.y;
            vzd = msg.current_twist.linear.z;
            qd.w() = msg.current_pose.orientation.w;
            qd.x() = msg.current_pose.orientation.x;
            qd.y() = msg.current_pose.orientation.y;
            qd.z() = msg.current_pose.orientation.z;
            wzd = msg.current_twist.angular.z;
            Point P = {xd, yd, zd};
            Point V = {vxd, vyd, vzd};

            trajd.push_back(P);
            trajvd.push_back(V);
            trajq.push_back(qd);
            trajwzd.push_back(wzd);
            std::cout << "xd: " << xd << " yd: " << yd << " zd: " << zd << " vxd: " << vxd << " vyd: " << vyd <<  "\n";

            for (int i = 0; i < N; i++)
            {

                xd = msg.future_pose.at(i).position.x;
                yd = msg.future_pose.at(i).position.y;
                zd = msg.future_pose.at(i).position.z;
                P = {xd, yd, zd};
                vxd = msg.future_twist.at(i).linear.x;
                vyd = msg.future_twist.at(i).linear.y;
                vzd = msg.future_twist.at(i).linear.z;
                V = {vxd, vyd, vzd};
                qd.w() = msg.future_pose.at(i).orientation.w;
                qd.x() = msg.future_pose.at(i).orientation.x;
                qd.y() = msg.future_pose.at(i).orientation.y;
                qd.z() = msg.future_pose.at(i).orientation.z;
                wzd = msg.future_twist.at(i).angular.z;

                trajd.push_back(P);
                trajvd.push_back(V);
                trajq.push_back(qd);
                trajwzd.push_back(wzd);
            }

            std::vector<double> u_opt = mpc.solve(pvec, vvec, qvec, wvec, trajd, trajvd, trajq, trajwzd, Q_cost);
            Eigen::Vector4d rotors_in = torque2rotors * Eigen::Vector4d{u_opt[0], u_opt[1], u_opt[2], u_opt[3]};
            geometry_msgs::msg::Wrench rotor1, rotor2, rotor3, rotor4;
            rotor1.force.z = rotors_in[0];
            rotor1.torque.z = -(km / kf) * rotors_in[0];
            rotor2.force.z = rotors_in[1];
            rotor2.torque.z = (km / kf) * rotors_in[1];
            rotor3.force.z = rotors_in[2];
            rotor3.torque.z = -(km / kf) * rotors_in[2];
            rotor4.force.z = rotors_in[3];
            rotor4.torque.z = (km / kf) * rotors_in[3];
            rotor1_publisher_->publish(rotor1);
            rotor2_publisher_->publish(rotor2);
            rotor3_publisher_->publish(rotor3);
            rotor4_publisher_->publish(rotor4);

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
        };

        g1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        g2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        rclcpp::SubscriptionOptions sub_options;
        rclcpp::PublisherOptions pub_options;

        sub_options.callback_group = g2;
        pub_options.callback_group = g2;
        auto qostype = rclcpp::SensorDataQoS();
        rotor1_publisher_ = this->create_publisher<geometry_msgs::msg::Wrench>(rotor_topic + "1", qostype, pub_options);
        rotor2_publisher_ = this->create_publisher<geometry_msgs::msg::Wrench>(rotor_topic + "2", qostype, pub_options);

        rotor3_publisher_ = this->create_publisher<geometry_msgs::msg::Wrench>(rotor_topic + "3", qostype, pub_options);

        rotor4_publisher_ = this->create_publisher<geometry_msgs::msg::Wrench>(rotor_topic + "4", qostype, pub_options);

        ref_sub = this->create_subscription<custom_interfaces::msg::ReferenceState>(ref_topic, 10, ref_callback, sub_options);
        state_client = this->create_client<gazebo_msgs::srv::GetEntityState>(state_service, rmw_qos_profile_services_default, g1);
        quadrotor_state_publisher_ = this->create_publisher<custom_interfaces::msg::ReferenceState>(state_topic, 10, pub_options);
        timer = create_wall_timer(5ms, timer_callback);
    }

    void get_params()
    {
        std::string path = ament_index_cpp::get_package_share_directory("robots_description") + "/" + "config";
        std::string file_name = quadrotor_name;
        std::string params_node = quadrotor_name;
        std::transform(params_node.begin(), params_node.end(), params_node.begin(),
                       ::toupper);
        params_node = params_node + "_PARAMS";
        try
        {

            YAML::Node config = YAML::LoadFile(path + "/" + file_name + "_params.yaml");

            km = config[params_node]["KM"].as<double>();
            kf = config[params_node]["KF"].as<double>();
            arm = config[params_node]["ARM"].as<double>() * sqrt(2.0) / 2.0;
            M = config[params_node]["M"].as<double>();
            t2w = config[params_node]["T2W"].as<double>();
            std::vector<std::vector<double>> J_v;
            J_v = config[params_node]["J"].as<std::vector<std::vector<double>>>();
            std::vector<double> J_0 = J_v.at(0), J_1 = J_v.at(1), J_2 = J_v.at(2);
            J = Eigen::Matrix3d{{J_0[0], J_0[1], J_0[2]},
                                {J_1[0], J_1[1], J_1[2]},
                                {J_2[0], J_2[1], J_2[2]}};
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';

            kf = 3.16e-10;
            km = 7.94e-12;
            arm = 0.325 * sqrt(2) / 2;
            M = 2.355;
            J = Eigen::Matrix3d{{0.01152, 0, 0}, {0, 0.01152, 0}, {0, 0, 0.0218}};
        }
        Eigen::Matrix4<double> rotors2torque{
            {-arm, arm, arm, -arm},
            {-arm, -arm, arm, arm},
            {-km / kf, +km / kf, -km / kf, km / kf},
            {1, 1, 1, 1}};
        torque2rotors = rotors2torque.inverse();
    }

    rclcpp::Publisher<geometry_msgs::msg::Wrench>::SharedPtr rotor1_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Wrench>::SharedPtr rotor2_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Wrench>::SharedPtr rotor3_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Wrench>::SharedPtr rotor4_publisher_;
    rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr quadrotor_state_publisher_;
    rclcpp::Subscription<custom_interfaces::msg::ReferenceState>::SharedPtr ref_sub;
    rclcpp::TimerBase::SharedPtr timer;

    custom_interfaces::msg::ReferenceState ref_msg;
    rclcpp::Client<gazebo_msgs::srv::GetEntityState>::SharedPtr state_client;

    rclcpp::CallbackGroup::SharedPtr g1;
    rclcpp::CallbackGroup::SharedPtr g2;

    Eigen::Matrix4<double> torque2rotors;
    double kf;
    double km;
    double arm;
    double M;

    Eigen::Matrix3<double> J;
    Eigen::Vector3<double> p = {0, 0, 0};
    Eigen::Vector3<double> v = {0, 0, 0};
    Eigen::Vector3<double> a = {0, 0, 0};
    Eigen::Vector3<double> w = {0, 0, 0};
    MPC mpc;
    std::vector<double> pvec = {0, 0, 0};
    std::vector<double> vvec = {0, 0, 0};
    std::vector<double> qvec = {1, 0, 0, 0};
    std::vector<double> wvec = {0, 0, 0};
    Eigen::Quaterniond Q{1.0, 0.0, 0.0, 0.0};
    double t2w;
    std::string quadrotor_name;
    int N;
    double dt;
    std::vector<double> Q_cost;
    std::vector<double> R_cost;
    std::string state_service;
    std::string ref_topic;
    std::string rotor_topic;
    std::string state_topic;
};

int main(int argc, char *argv[])
{
    // pose_cont cont;
    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor exec;
    auto node = std::make_shared<quadrotor_mpc>();
    exec.add_node(node);
    RCLCPP_INFO(node->get_logger(), "Starting client node, shut down with CTRL-C");
    exec.spin();
    RCLCPP_INFO(node->get_logger(), "Keyboard interrupt, shutting down.\n");

    // cont.take_off();
    // rclcpp::spin();
    rclcpp::shutdown();
    return 0;
}

#include <rclcpp/rclcpp.hpp>
#include <custom_interfaces/msg/reference_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>


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
            if (t<8)
            {
                t+=DT;
            }
            else
            {
                t=0;
            }
            
            custom_interfaces::msg::ReferenceState reference_state_msg;
            reference_state_msg.header.stamp = this->now();
            reference_state_msg.current_pose.position.x = 0.0;
            reference_state_msg.current_pose.position.y = 0.0;
            reference_state_msg.current_pose.position.z = 2.0;
            reference_state_msg.current_pose.orientation.x = 0.0;
            reference_state_msg.current_pose.orientation.y = 0.0;
            reference_state_msg.current_pose.orientation.z = sin((M_PI / 8));
            reference_state_msg.current_pose.orientation.w = cos((M_PI / 8));
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
        auto cam_callback = [this](const sensor_msgs::msg::Image::SharedPtr msg) -> void
        {
            cv_bridge::CvImagePtr cv_ptr;
            try
            {
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                img = cv_ptr->image;
               
                
                cv::Mat img_copy = img.clone();
                cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
                cv::aruco::detectMarkers (img,dictionary, markerCorners,markerIds);
                if(markerIds.size()>0){
                    cv::aruco::drawDetectedMarkers(img_copy, markerCorners, markerIds);
                    std::vector<cv::Vec3d> rvecs, tvecs;
                    cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.05, cam_matrix, cv::noArray(), rvecs, tvecs);
                    for(int i=0; i<markerIds.size(); i++){
                        cv::aruco::drawAxis(img_copy, cam_matrix, cv::noArray(), rvecs[i], tvecs[i], 0.1);
                    }
                }


                cv::imshow("Image", img_copy);
                cv::waitKey(1);
            }
            catch (cv_bridge::Exception &e)
            {
                RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            }
        };
        publisher_ref = this->create_publisher<custom_interfaces::msg::ReferenceState>(
            ref_topic,
            10 
        );
        timer = this->create_wall_timer(std::chrono::duration<double>(this->DT),
                                        timer_callback);
        subscription_cam = this->create_subscription<sensor_msgs::msg::Image>(
            "/quadrotor/camera1/image_raw",
            10, cam_callback);
        subscription_cam_info=this->create_subscription<sensor_msgs::msg::CameraInfo>(
            "/quadrotor/camera1/camera_info",
            10,
            [this](const sensor_msgs::msg::CameraInfo::SharedPtr msg) -> void
            {
                cam_matrix = cv::Mat(3, 3, CV_64F);
                cam_matrix.at<double>(0, 0) = msg->k[0];
                cam_matrix.at<double>(0, 1) = msg->k[1];
                cam_matrix.at<double>(0, 2) = msg->k[2];
                cam_matrix.at<double>(1, 0) = msg->k[3];
                cam_matrix.at<double>(1, 1) = msg->k[4];
                cam_matrix.at<double>(1, 2) = msg->k[5];
                cam_matrix.at<double>(2, 0) = msg->k[6];
                cam_matrix.at<double>(2, 1) = msg->k[7];
                cam_matrix.at<double>(2, 2) = msg->k[8];
            });

    }

private:
    rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr publisher_ref;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_cam;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr subscription_cam_info;

    int n_future;
    double dt_future;
    double publish_rate;
    std::string ref_topic;
    double DT;
    rclcpp::TimerBase::SharedPtr timer;
    cv::Mat img;
    cv::Mat cam_matrix;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    std::vector<int> markerIds;
    double t=0;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<QuadrotorReferencePublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

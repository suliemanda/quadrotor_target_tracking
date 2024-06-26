#include <rclcpp/rclcpp.hpp>
#include <custom_interfaces/msg/reference_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <Eigen/Dense>

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
        this->declare_parameter("state_topic", "quadrotor_state");
        this->declare_parameter("cam_freq", 30.0);
        n_future = this->get_parameter("N").as_int();
        state_topic = this->get_parameter("state_topic").as_string();

        dt_future = this->get_parameter("dt").as_double();

        publish_rate = this->get_parameter("publish_rate").as_double();
        ref_topic = this->get_parameter("reference_topic").as_string();
        cam_freq = this->get_parameter("cam_freq").as_double();

        // Control the publishing rate
        // Hz
        DT = 1.0 / this->publish_rate; // seconds
        double dt_measure = 1.0 / this->cam_freq;
        t_start = this->now().seconds();
        KF.init(6, 2);
        KF.transitionMatrix = (cv::Mat_<float>(6, 6) << 1, 0, dt_measure, 0, 0.5 * dt_measure * dt_measure, 0,
                               0, 1, 0, dt_measure, 0, 0.5 * dt_measure * dt_measure,
                               0, 0, 1, 0, dt_measure, 0,
                               0, 0, 0, 1, 0, dt_measure,
                               0, 0, 0, 0, 1, 0,
                               0, 0, 0, 0, 0, 1);
        KF.measurementMatrix = (cv::Mat_<float>(2, 6) << 1, 0, 0, 0, 0, 0,
                                0, 1, 0, 0, 0, 0);
        double sigma_jerk_x = 0.1;
        double sigma_jerk_y = 0.1;

        KF.processNoiseCov = (cv::Mat_<float>(6, 6) << (1.0 / 9.0) * pow(dt_measure, 6) * sigma_jerk_x, 0, (1.0 / 6.0) * pow(dt_measure, 5) * sigma_jerk_x, 0, 1.0 / 3.0 * pow(dt_measure, 4) * sigma_jerk_x, 0,
                              0, (1.0 / 9.0) * pow(dt_measure, 6) * sigma_jerk_y, 0, (1.0 / 6.0) * pow(dt_measure, 5) * sigma_jerk_y, 0, 1.0 / 3.0 * pow(dt_measure, 4) * sigma_jerk_y,
                              (1.0 / 6.0) * pow(dt_measure, 5) * sigma_jerk_x, 0, (1.0 / 4.0) * pow(dt_measure, 4) * sigma_jerk_x, 0, 1.0 / 2.0 * pow(dt_measure, 3) * sigma_jerk_x, 0,
                              0, (1.0 / 6.0) * pow(dt_measure, 5) * sigma_jerk_y, 0, (1.0 / 4.0) * pow(dt_measure, 4) * sigma_jerk_y, 0, 1.0 / 2.0 * pow(dt_measure, 3) * sigma_jerk_y,
                              (1.0 / 3.0) * pow(dt_measure, 4) * sigma_jerk_x, 0, (1.0 / 2.0) * pow(dt_measure, 3) * sigma_jerk_x, 0, dt_measure * dt_measure * sigma_jerk_x, 0,
                              0, (1.0 / 3.0) * pow(dt_measure, 4) * sigma_jerk_y, 0, (1.0 / 2.0) * pow(dt_measure, 3) * sigma_jerk_y, 0, dt_measure * dt_measure * sigma_jerk_y);
        KF.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1e-2, 0, 0, 1e-2);
        KF.statePost = (cv::Mat_<float>(6, 1) << 0, 0, 0, 0, 0, 0);

        RCLCPP_INFO(this->get_logger(), "Reference publisher node initialized");
        auto state_callback = [this](const custom_interfaces::msg::ReferenceState::SharedPtr msg) -> void
        {
            P.x() = msg->current_pose.position.x;
            P.y() = msg->current_pose.position.y;
            P.z() = msg->current_pose.position.z;
            Eigen::Quaterniond Q;
            Q.x() = msg->current_pose.orientation.x;
            Q.y() = msg->current_pose.orientation.y;
            Q.z() = msg->current_pose.orientation.z;
            Q.w() = msg->current_pose.orientation.w;
            R = Q.toRotationMatrix();
            T.block<3, 3>(0, 0) = R;
            T.block<3, 1>(0, 3) = P;
            T(3, 0) = 0.0;
            T(3, 1) = 0.0;
            T(3, 2) = 0.0;
            T(3, 3) = 1.0;
        };
        auto timer_callback = [this]() -> void
        {
            t = this->now().seconds() - t_start;
            custom_interfaces::msg::ReferenceState reference_state_msg;
            reference_state_msg.current_pose.position.x = x;
            reference_state_msg.current_pose.position.y = y;
            reference_state_msg.current_pose.position.z = 8.0;
            reference_state_msg.current_pose.orientation.x = 0.0;
            reference_state_msg.current_pose.orientation.y = 0.0;
            reference_state_msg.current_pose.orientation.z = 0.0;
            reference_state_msg.current_pose.orientation.w = 1.0;
            reference_state_msg.current_twist.linear.x = vx;
            reference_state_msg.current_twist.linear.y = vy;
            reference_state_msg.current_twist.linear.z = 0.0;
            reference_state_msg.current_twist.angular.x = 0.0;
            reference_state_msg.current_twist.angular.y = 0.0;
            reference_state_msg.current_twist.angular.z = 0.0;

            for (int i = 0; i < n_future; i++)
            {
                geometry_msgs::msg::Pose future_pose;
                geometry_msgs::msg::Twist future_twist;

                double next_x = x + vx * (i + 1) * dt_future + 0.5 * ax * pow((i + 1) * dt_future, 2);
                double next_y = y + vy * (i + 1) * dt_future + 0.5 * ay * pow((i + 1) * dt_future, 2);
                double next_vx = vx + ax * (i + 1) * dt_future;
                double next_vy = vy + ay * (i + 1) * dt_future;

                future_pose.position.x = next_x;
                future_pose.position.y = next_y;
                future_pose.position.z = 8.0;
                future_pose.orientation.x = 0.0;
                future_pose.orientation.y = 0.0;
                future_pose.orientation.z = 0.0;
                future_pose.orientation.w = 0.0;
                future_twist.linear.x = next_vx;
                future_twist.linear.y = next_vy;
                future_twist.linear.z = 0.0;
                future_twist.angular.x = 0.0;
                future_twist.angular.y = 0.0;
                future_twist.angular.z = 0.0;
                reference_state_msg.future_pose.push_back(future_pose);
                reference_state_msg.future_twist.push_back(future_twist);
            }
            x = x + vx * DT + 0.5 * ax * pow(DT, 2);
            y = y + vy * DT + 0.5 * ay * pow(DT, 2);
            vx = vx + ax * DT;
            vy = vy + ay * DT;

            reference_state_msg.header.stamp = this->now();

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
                cv::aruco::detectMarkers(img, dictionary, markerCorners, markerIds);
                if (markerIds.size() > 0)
                {
                    cv::aruco::drawDetectedMarkers(img_copy, markerCorners, markerIds);
                    std::vector<cv::Vec3d> rvecs, tvecs;
                    cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.5, cam_matrix, cv::noArray(), rvecs, tvecs);
                    for (int i = 0; i < markerIds.size(); i++)
                    {
                        cv::aruco::drawAxis(img_copy, cam_matrix, cv::noArray(), rvecs[i], tvecs[i], 0.1);
                    }
                    Eigen::Vector3d P_marker;
                    P_marker.x() = tvecs[0][0];
                    P_marker.y() = tvecs[0][1];
                    P_marker.z() = tvecs[0][2];

                    Eigen::Matrix3d R_marker;
                    cv::Mat R_marker_cv;
                    cv::Rodrigues(rvecs[0], R_marker_cv);
                    R_marker << R_marker_cv.at<double>(0, 0), R_marker_cv.at<double>(0, 1), R_marker_cv.at<double>(0, 2),
                        R_marker_cv.at<double>(1, 0), R_marker_cv.at<double>(1, 1), R_marker_cv.at<double>(1, 2),
                        R_marker_cv.at<double>(2, 0), R_marker_cv.at<double>(2, 1), R_marker_cv.at<double>(2, 2);
                    // cv::cv2eigen(R_marker_cv, R_marker);
                    Eigen::Matrix4d T_marker;
                    T_marker.block<3, 3>(0, 0) = R_marker;
                    T_marker.block<3, 1>(0, 3) = P_marker;
                    T_marker(3, 0) = 0.0;
                    T_marker(3, 1) = 0.0;
                    T_marker(3, 2) = 0.0;
                    T_marker(3, 3) = 1.0;
                    Eigen::Matrix4d T_cam_to_body;
                    T_cam_to_body << 0, -1, 0, 0,
                        -1, 0, 0, 0,
                        0, 0, -1, 0,
                        0, 0, 0, 1;

                    Eigen::Matrix4d T_marker_to_world = T * T_cam_to_body * T_marker;
                    P_marker_to_world = T_marker_to_world.block<3, 1>(0, 3);

                    KF.correct((cv::Mat_<float>(2, 1) << P_marker_to_world.x(), P_marker_to_world.y()));
                    cv::Mat prediction = KF.predict();
                    // std::cout << "Prediction: " << prediction << std::endl;
                    x = prediction.at<float>(0);
                    y = prediction.at<float>(1);
                    vx = prediction.at<float>(2);
                    vy = prediction.at<float>(3);
                    ax = prediction.at<float>(4);
                    ay = prediction.at<float>(5);
                }
                else{
                    // KF.correct((cv::Mat_<float>(2, 1) << P_marker_to_world.x(), P_marker_to_world.y()));
                    cv::Mat prediction = KF.predict();
                    // std::cout << "Prediction: " << prediction << std::endl;
                    x = prediction.at<float>(0);
                    y = prediction.at<float>(1);
                    vx = prediction.at<float>(2);
                    vy = prediction.at<float>(3);
                    ax = prediction.at<float>(4);
                    ay = prediction.at<float>(5);


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
            10);
        timer = this->create_wall_timer(std::chrono::duration<double>(this->DT),
                                        timer_callback);
        subscription_cam = this->create_subscription<sensor_msgs::msg::Image>(
            "/quadrotor/camera1/image_raw",
            10, cam_callback);
        subscription_cam_info = this->create_subscription<sensor_msgs::msg::CameraInfo>(
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
        state_subscription = this->create_subscription<custom_interfaces::msg::ReferenceState>(state_topic, 10, state_callback);
    }

private:
    rclcpp::Publisher<custom_interfaces::msg::ReferenceState>::SharedPtr publisher_ref;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_cam;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr subscription_cam_info;
    rclcpp::Subscription<custom_interfaces::msg::ReferenceState>::SharedPtr state_subscription;

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
    std::string state_topic;
    Eigen::Vector3d P;
    Eigen::Matrix3d R;
    Eigen::Matrix4d T;
    cv::KalmanFilter KF;
    double ax = 0.0;
    double ay = 0.0;
    double vx = 0.0;
    double vy = 0.0;
    double x = 0.0;
    double y = 0.0;
    double cam_freq;
    double t = 0;
    double t_start;
    Eigen::Vector3d P_marker_to_world;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<QuadrotorReferencePublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

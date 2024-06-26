# Message Type for the desired quadrotor state, supports receding horizon controllers by providing a list of future states

std_msgs/Header header
geometry_msgs/Pose current_pose
geometry_msgs/Twist current_twist

uint32 n # number of future states (for receding horizon controllers)
float32 dt # time between future states
geometry_msgs/Pose[] future_pose
geometry_msgs/Twist[] future_twist
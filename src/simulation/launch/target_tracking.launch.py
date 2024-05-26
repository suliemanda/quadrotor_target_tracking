from launch import LaunchDescription, LaunchContext
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument,ExecuteProcess
from launch.substitutions import LaunchConfiguration, TextSubstitution, PythonExpression

from ament_index_python.packages import get_package_share_directory
import yaml
import os


def generate_launch_description():
    simulation_share_dir=get_package_share_directory('simulation')
    config_file = os.path.join(simulation_share_dir,'config', 'simulation.yaml')
    with open(config_file, "r") as stream:
        try:
            parameters = yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            parameters = dict()
    mpc_controller_parameters=parameters['MPCController']
    ref_parameters=parameters['ReferencePublisher']
    gazebo_parameters=parameters['GazeoSimulation']
    world_file_name=gazebo_parameters['world_description']
    quadrotor_file_name=gazebo_parameters['quadrotor_description']
    world_file= os.path.join(simulation_share_dir,'world', world_file_name+'.world')
    description_share_dir=get_package_share_directory('robots_description')
    quadrotor_urdf=os.path.join(description_share_dir,'robots', quadrotor_file_name+'.urdf')





    mpc_controller_node = Node(
        package='quadrotor_control',
        executable='quadrotor_mpc',
        output='screen',
        parameters=[mpc_controller_parameters]
    )

    reference_publisher_node = Node(
        package='quadrotor_control',
        executable='quadrotor_vision',
        output='screen',
        parameters=[ref_parameters]

    )
    # quadrotor_description_publisher=  Node(
    #         package='robot_state_publisher',
    #         executable='robot_state_publisher',
    #         name='robot_state_publisher',
    #         output='screen',
    #         arguments=[quadrotor_urdf])
    spawn_quadrotor=Node( package='gazebo_ros',
            executable='spawn_entity.py',
            name='urdf_spawner',
            output='screen',
            arguments=["-file", quadrotor_urdf, "-entity",quadrotor_file_name ])



    return LaunchDescription([
        # quadrotor_description_publisher,
        ExecuteProcess(
            cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so','-s', 'libgazebo_ros_init.so',world_file],  
            output='screen'),
                               spawn_quadrotor,
                            #   mpc_controller_node,
                               reference_publisher_node, 
                              ])
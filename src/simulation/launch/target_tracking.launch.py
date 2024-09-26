from launch import LaunchDescription, LaunchContext
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument,ExecuteProcess
from launch.substitutions import LaunchConfiguration, TextSubstitution, PythonExpression
from launch.event_handlers import OnExecutionComplete, OnProcessExit, OnProcessIO, OnProcessStart, OnShutdown
from launch.actions import (DeclareLaunchArgument, EmitEvent, ExecuteProcess,
                            LogInfo, RegisterEventHandler, TimerAction)
from ament_index_python.packages import get_package_share_directory 
import yaml
import os
import xacro


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
    ground_robot_file_name="ground_robot"
    world_file= os.path.join(simulation_share_dir,'world', world_file_name+'.world')
    description_share_dir=get_package_share_directory('robots_description')
    urdf_shar_dir=os.path.join(description_share_dir,'robots')
    quadrotor_xacro=os.path.join(description_share_dir,'robots', quadrotor_file_name+'.xacro')
    ground_robot_xacro=os.path.join(description_share_dir,'robots',ground_robot_file_name+'.xacro')
    ground_robot_urdf=xacro.process_file(ground_robot_xacro).toxml()
    quadrotor_urdf=xacro.process_file(quadrotor_xacro).toxml()
    





    mpc_controller_node = Node(
        package='quadrotor_control',
        executable='quadrotor_mpc',
        output='screen',
        parameters=[mpc_controller_parameters]
    )
    ground_robot_control_node = Node(
        package='ground_robot_control',
        executable='ground_robot_PID',
        output='screen',
    )

    quadrotor_reference_publisher_node = Node(
        package='quadrotor_control',
        executable='quadrotor_vision',
        output='screen',
        parameters=[ref_parameters]

    )
    ground_robot_reference_publisher_node=Node(
        package='ground_robot_control',
        executable='ground_robot_reference',
        output='screen',


    )
    quadrotor_description_publisher=  Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            namespace='quadrotor',
            parameters=[
                {
                    'robot_description': quadrotor_urdf,
                }
            ],
            
            )
    
    GRobot_description_publisher=  Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            namespace='ground_robot',
            parameters=[
                {
                    'robot_description': ground_robot_urdf,
                }
            ])
    start_sim= ExecuteProcess(
        cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so','-s', 'libgazebo_ros_init.so',world_file],  
        output='screen')
    spawn_quad=   ExecuteProcess(
        cmd=['ros2', 'run', 'gazebo_ros', 'spawn_entity.py','-topic','/quadrotor/robot_description',"-x", '-1',"-y",'0',"-z", '0',"-entity",quadrotor_file_name],  
        output='screen')
    spawn_gr=ExecuteProcess(
        cmd=['ros2', 'run', 'gazebo_ros', 'spawn_entity.py','-topic', '/ground_robot/robot_description',"-x", '0',"-y",'0',"-z", '0.2',"-entity",ground_robot_file_name],  
        output='screen')
    return LaunchDescription([
        start_sim,
        RegisterEventHandler(
            OnProcessStart(
                target_action=start_sim,
                on_start=[
                    LogInfo(msg='simulation started'),
                    spawn_quad,
                    quadrotor_description_publisher,
                    quadrotor_reference_publisher_node,
                    mpc_controller_node,
                    ])),
                    
            RegisterEventHandler(
                OnExecutionComplete( 
                    target_action=spawn_quad,
                    on_completion=[ LogInfo(msg='quad rotor spawned'),
                                   spawn_gr,GRobot_description_publisher,
                                   ground_robot_control_node,
                                   ground_robot_reference_publisher_node])),

            
                              ])

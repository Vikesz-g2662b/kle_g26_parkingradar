from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='kle_g26_parkingradar',
            executable='distance_sensor',
            output='screen',
        ),
        Node(
            package='kle_g26_parkingradar',
            executable='parking_radar',
            output='screen',
        ),
    ])

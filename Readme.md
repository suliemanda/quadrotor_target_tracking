# Quadrotor Target Tracking

<!-- [![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT) -->

## Description

This package was created as a course project for the robot programming course at ITMO university

## Features

- Simulation and control of quadrotor's movement using rotors thrust as control input
- Trajectory control of differintial drive mobile robot
- Target detection and trajectory estimation
- Target tracking algorithms

## Installation

1. Clone the repository:

    ```shell
    git clone https://github.com/suliemanda/quadrotor_target_tracking.git
    ```

2. Build and run docker file:

    ```shell
    cd quadrotor_target_tracking
    ./build_docker.sh
    ./run_docker.sh
    ```

## Usage

1. Inside docker, build ros2 packages:

    ```shell
    ./build_packages.bash

    ```

2. Launch the project.
    ```shell
    ros2 launch simulation target_tracking.launch.py
    ```


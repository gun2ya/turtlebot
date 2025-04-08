#!/bin/bash

colcon build
source install/setup.bash
ros2 run wanderbot go_scan
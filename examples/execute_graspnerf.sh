ROBOT_ADDR=172.16.0.2
./move_to_neutral $ROBOT_ADDR
./gripper_homing $ROBOT_ADDR
./generate_cartesian_pose_motion $ROBOT_ADDR $FNAME 7
./grasp_object $ROBOT_ADDR 0 0.02
./move_to_neutral $ROBOT_ADDR


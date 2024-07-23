ROBOT_ADDR=172.16.0.2
./move_to_neutral $ROBOT_ADDR
./gripper_homing $ROBOT_ADDR
./franka_grasp_with_traj $ROBOT_ADDR trajectories/in_motion.txt 3 1
./grasp_object $ROBOT_ADDR 0 0.02
./franka_grasp_with_traj $ROBOT_ADDR trajectories/out_motion.txt 3 1
./move_over_box $ROBOT_ADDR
./gripper_homing $ROBOT_ADDR
./move_to_neutral $ROBOT_ADDR


ROBOT_ADDR=172.16.0.2
./move_to_neutral $ROBOT_ADDR
./gripper_homing $ROBOT_ADDR
./grasp_object $ROBOT_ADDR 0 0.01
./franka_grasp_with_traj $ROBOT_ADDR trajectories/interact/motion.txt 3 1
./franka_grasp_with_traj $ROBOT_ADDR trajectories/interact/motion2.txt 3 1
./move_to_neutral $ROBOT_ADDR
./gripper_homing $ROBOT_ADDR


#include <cmath>
#include <iostream>
#include <sstream>

#include <franka/exception.h>
#include <franka/robot.h>
#include <franka/gripper.h>
#include <franka/control_types.h>

#include "examples_common.h"

//#include "ros/ros.h"
//#include "std_msgs/String.h"

std::array<double, 16> euler_to_rot(double x, double y, double z){
    double s1 = std::sin(x);    double c1 = std::cos(x);
    double s2 = std::sin(y);    double c2 = std::cos(y);
    double s3 = std::sin(z);    double c3 = std::cos(z);
    std::array<double, 16> SE3= {{c3*c2,          s3*c2,            -s2, 0,
                                  c3*s2*s1-s3*c1, s3*s2*s1+c3*c1, c2*s1, 0,
                                  c3*s2*c1+s3*s1, s3*s2*c1-c3*s1, c2*c1, 0,
                                  0.3,0,0.5,1
                                  }};
    return SE3;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << std::endl;
    return -1;
  }

  //ros publisher setting
  /*
  ros::init(argc, argv, "state_pub");
  ros::NodeHandle n;
  ros::Publisher EE_Cart_Pos_pub = n.advertise<std_msgs::String>("EE_Cart_Pos", 1000);
  */
  try {
      franka::Robot robot(argv[1], franka::RealtimeConfig::kIgnore);
      franka::Gripper gripper(argv[1]);
      
      std::array<double, 7> init_joint = {{0, -M_PI_2, 0, -3 * M_PI_4, 0, M_PI_4, M_PI_4}};
      MotionGenerator motion_generator(0.3, init_joint);
      robot.control(motion_generator);
      gripper.move(0.08, 0.08);

      setDefaultBehavior(robot);

      robot.setCollisionBehavior(
          {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
          {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
          {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
          {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});

      for(int i =0 ; i<6; i++){
        std::array<double, 7> start_joint = {{0, -M_PI_2, 0, -3 * M_PI_4, 0, M_PI_4, M_PI_4}};
        std::array<double, 7> ready_joint = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
        
        MotionGenerator motion_generator_start(0.3, start_joint);
        robot.control(motion_generator_start);

        std::array<double, 16> initial_pose;
        double time = 0.0;    //[s]
        double oper_time;     //[s]
        double max_vel = 0.1; //[m/s]
        /*
        std::array<double, 16> target_pose = {1,  0, 0, 0,
                                              0, -1, 0, 0,
                                              0,  0, -1, 0,
                                              0,  0, 0.5, 1};
        */
        std::array<double, 16> target_pose = euler_to_rot(-M_PI,0,0);

        std::cout << "x pos : ";       //[m]
        std::cin >> target_pose[12];
        std::cout << "y pos : ";       //[m]
        std::cin >> target_pose[13];
        std::cout << "z pos : ";       //[m]
        std::cin >> target_pose[14];

  
        MotionGenerator motion_generator_ready(0.3, ready_joint);
        robot.control(motion_generator_ready);

        std::cout << "Excuting Motion.." << std::endl;
        try {
          robot.control([&time, &oper_time, &initial_pose, &target_pose, &max_vel](const franka::RobotState& robot_state,
                                              franka::Duration period) -> franka::CartesianPose {

            time += period.toSec();

            if (time == 0.0) {
              initial_pose = robot_state.O_T_EE_c;
              oper_time = pow(pow(target_pose[12]-initial_pose[12], 2) + pow(target_pose[13]-initial_pose[13], 2) + pow(target_pose[14]-initial_pose[14], 2),0.5) / max_vel;
            }

            double angle = M_PI / 4 * (1 - std::cos(M_PI / oper_time * time));

            std::array<double, 16> new_pose = initial_pose;

            for(int i = 0; i < 15; i++){
              new_pose[i] += (target_pose[i] - initial_pose[i]) * std::sin(angle);
            }

            //ros publish
            /*
            std_msgs::String msg;
            std::stringstream ss;
            ss << "x : " << new_pose[12] << ", y : " << new_pose[13] << ", z : " << new_pose[14];
            msg.data = ss.str();
            EE_Cart_Pos_pub.publish(msg);
            ros::spinOnce();
            */
            if (time >= oper_time) {
              std::cout << "Finished, Operation Time : " << time << std::endl;
              return franka::MotionFinished(new_pose);
            }
            return new_pose;
          });
        } catch (const franka::ControlException& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Running error recovery..." << std::endl;
        robot.automaticErrorRecovery();
        }
        gripper.homing();
        gripper.stop();
        gripper.move(0.08, 0.08);
        gripper.grasp(0.0, 0.1, 60);
      }
    } catch (const franka::Exception& e) {
      std::cout << e.what() << std::endl;
      return -1;
    }
  return 0;
}
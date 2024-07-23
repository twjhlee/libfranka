# libfranka: C++ library for Franka Emika research robots

[![Build Status][travis-status]][travis]
[![codecov][codecov-status]][codecov]

With this library, you can control research versions of Franka Emika robots. See the [Franka Control Interface (FCI) documentation][fci-docs] for more information about what `libfranka` can do and how to set it up. The [generated API documentation][api-docs] also gives an overview of its capabilities.

## License

`libfranka` is licensed under the [Apache 2.0 license][apache-2.0].

[apache-2.0]: https://www.apache.org/licenses/LICENSE-2.0.html
[api-docs]: https://frankaemika.github.io/libfranka
[fci-docs]: https://frankaemika.github.io/docs
[travis-status]: https://travis-ci.org/frankaemika/libfranka.svg?branch=master
[travis]: https://travis-ci.org/frankaemika/libfranka
[codecov-status]: https://codecov.io/gh/frankaemika/libfranka/branch/master/graph/badge.svg
[codecov]: https://codecov.io/gh/frankaemika/libfranka

## Newly Added Code for Panda INMC
    cd examples
### Capturing new images with panda
    ./follow_good_traj 172.16.0.2 <dataset-name> <path-to-joint-trajectory-txt> <trajectory-step>
### Nudging objects
    ./nudge_franka.sh
### Overlapping images before change and now
    python3 overlap.py <path-to-img>
### Moving the neutral pose
    ./move_to_neutral 172.16.0.2
### Application to move, close, open gripper
    ./cartesian_pose_motion.app 172.16.0.2
### Writing joint poses - capturing
    ./print_joint_poses 172.16.0.2 <path-to-save-txt>
### Grasping
    ./execute_grasp.sh


## Troubleshooting
### Building - usually change examples folder only
    cd examples
    make
### Checking current kernel
    uname - r #result needs to contain "rt"
### Communication test
    ./communication_test 172.16.0.2


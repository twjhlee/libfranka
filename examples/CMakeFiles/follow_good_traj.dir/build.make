# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/panda/libfranka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/panda/libfranka

# Include any dependencies generated for this target.
include examples/CMakeFiles/follow_good_traj.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/follow_good_traj.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/follow_good_traj.dir/flags.make

examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o: examples/CMakeFiles/follow_good_traj.dir/flags.make
examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o: examples/follow_good_traj.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o"
	cd /home/panda/libfranka/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o -c /home/panda/libfranka/examples/follow_good_traj.cpp

examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.i"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/panda/libfranka/examples/follow_good_traj.cpp > CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.i

examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.s"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/panda/libfranka/examples/follow_good_traj.cpp -o CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.s

# Object files for target follow_good_traj
follow_good_traj_OBJECTS = \
"CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o"

# External object files for target follow_good_traj
follow_good_traj_EXTERNAL_OBJECTS =

examples/follow_good_traj: examples/CMakeFiles/follow_good_traj.dir/follow_good_traj.cpp.o
examples/follow_good_traj: examples/CMakeFiles/follow_good_traj.dir/build.make
examples/follow_good_traj: examples/libexamples_common.a
examples/follow_good_traj: libfranka.so.0.8.0
examples/follow_good_traj: examples/CMakeFiles/follow_good_traj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable follow_good_traj"
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/follow_good_traj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/follow_good_traj.dir/build: examples/follow_good_traj

.PHONY : examples/CMakeFiles/follow_good_traj.dir/build

examples/CMakeFiles/follow_good_traj.dir/clean:
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -P CMakeFiles/follow_good_traj.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/follow_good_traj.dir/clean

examples/CMakeFiles/follow_good_traj.dir/depend:
	cd /home/panda/libfranka && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka/examples/CMakeFiles/follow_good_traj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/follow_good_traj.dir/depend

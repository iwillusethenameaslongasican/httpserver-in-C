# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /home/jackson/software/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/jackson/software/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/c++code/httpServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/c++code/httpServer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/httpdServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/httpdServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/httpdServer.dir/flags.make

CMakeFiles/httpdServer.dir/httpd.cpp.o: CMakeFiles/httpdServer.dir/flags.make
CMakeFiles/httpdServer.dir/httpd.cpp.o: ../httpd.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/c++code/httpServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/httpdServer.dir/httpd.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpdServer.dir/httpd.cpp.o -c /home/c++code/httpServer/httpd.cpp

CMakeFiles/httpdServer.dir/httpd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpdServer.dir/httpd.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/c++code/httpServer/httpd.cpp > CMakeFiles/httpdServer.dir/httpd.cpp.i

CMakeFiles/httpdServer.dir/httpd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpdServer.dir/httpd.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/c++code/httpServer/httpd.cpp -o CMakeFiles/httpdServer.dir/httpd.cpp.s

CMakeFiles/httpdServer.dir/httpd.cpp.o.requires:

.PHONY : CMakeFiles/httpdServer.dir/httpd.cpp.o.requires

CMakeFiles/httpdServer.dir/httpd.cpp.o.provides: CMakeFiles/httpdServer.dir/httpd.cpp.o.requires
	$(MAKE) -f CMakeFiles/httpdServer.dir/build.make CMakeFiles/httpdServer.dir/httpd.cpp.o.provides.build
.PHONY : CMakeFiles/httpdServer.dir/httpd.cpp.o.provides

CMakeFiles/httpdServer.dir/httpd.cpp.o.provides.build: CMakeFiles/httpdServer.dir/httpd.cpp.o


# Object files for target httpdServer
httpdServer_OBJECTS = \
"CMakeFiles/httpdServer.dir/httpd.cpp.o"

# External object files for target httpdServer
httpdServer_EXTERNAL_OBJECTS =

httpdServer: CMakeFiles/httpdServer.dir/httpd.cpp.o
httpdServer: CMakeFiles/httpdServer.dir/build.make
httpdServer: CMakeFiles/httpdServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/c++code/httpServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable httpdServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/httpdServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/httpdServer.dir/build: httpdServer

.PHONY : CMakeFiles/httpdServer.dir/build

CMakeFiles/httpdServer.dir/requires: CMakeFiles/httpdServer.dir/httpd.cpp.o.requires

.PHONY : CMakeFiles/httpdServer.dir/requires

CMakeFiles/httpdServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/httpdServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/httpdServer.dir/clean

CMakeFiles/httpdServer.dir/depend:
	cd /home/c++code/httpServer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/c++code/httpServer /home/c++code/httpServer /home/c++code/httpServer/cmake-build-debug /home/c++code/httpServer/cmake-build-debug /home/c++code/httpServer/cmake-build-debug/CMakeFiles/httpdServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/httpdServer.dir/depend


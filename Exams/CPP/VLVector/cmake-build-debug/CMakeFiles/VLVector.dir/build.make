# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /cygdrive/c/Users/BarM/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/BarM/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/VLVector.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/VLVector.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VLVector.dir/flags.make

CMakeFiles/VLVector.dir/main.cpp.o: CMakeFiles/VLVector.dir/flags.make
CMakeFiles/VLVector.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/VLVector.dir/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VLVector.dir/main.cpp.o -c "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/main.cpp"

CMakeFiles/VLVector.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VLVector.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/main.cpp" > CMakeFiles/VLVector.dir/main.cpp.i

CMakeFiles/VLVector.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VLVector.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/main.cpp" -o CMakeFiles/VLVector.dir/main.cpp.s

# Object files for target VLVector
VLVector_OBJECTS = \
"CMakeFiles/VLVector.dir/main.cpp.o"

# External object files for target VLVector
VLVector_EXTERNAL_OBJECTS =

VLVector.exe: CMakeFiles/VLVector.dir/main.cpp.o
VLVector.exe: CMakeFiles/VLVector.dir/build.make
VLVector.exe: CMakeFiles/VLVector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable VLVector.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VLVector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VLVector.dir/build: VLVector.exe

.PHONY : CMakeFiles/VLVector.dir/build

CMakeFiles/VLVector.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/VLVector.dir/cmake_clean.cmake
.PHONY : CMakeFiles/VLVector.dir/clean

CMakeFiles/VLVector.dir/depend:
	cd "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector" "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector" "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug" "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug" "/cygdrive/d/Documents/School/C++ & C/Exams/CPP/VLVector/cmake-build-debug/CMakeFiles/VLVector.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/VLVector.dir/depend


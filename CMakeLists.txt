cmake_minimum_required(VERSION 2.8.3)
project(ft_sensor_fc6d50)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -O3 -march=native")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  # genmsg
)

# add_action_files(
#   DIRECTORY action
#   FILES ActuateHand.action
# )

# add_service_files(
#   DIRECTORY srv 
#   FILES hand_state.srv
# )

generate_messages(
  DEPENDENCIES 
  std_msgs
)

catkin_package(
  INCLUDE_DIRS
  CATKIN_DEPENDS
  message_runtime
  roscpp
  rospy
  std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
  include/ft_sensor_fc6d50
)

add_library(
  ${PROJECT_NAME}_lib SHARED
  # src/delay.cpp
  src/udp_adapter.cpp
  src/udp_interface.cpp
)

add_dependencies(${PROJECT_NAME}_lib ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(${PROJECT_NAME}_lib
  ${catkin_LIBRARIES}
)

add_executable(ft_node src/node.cpp)
target_link_libraries(ft_node ${PROJECT_NAME}_lib)
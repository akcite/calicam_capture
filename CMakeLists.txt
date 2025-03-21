cmake_minimum_required(VERSION 2.8.12.2)
project(calicam)

set(CMAKE_CXX_STANDARD 14)
SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")

find_package( OpenCV )
find_package(catkin REQUIRED COMPONENTS
        roscpp
        image_transport
        std_msgs
        cv_bridge
        )

include_directories(
        ${catkin_INCLUDE_DIRS}
)

catkin_package()

####### end ########


add_executable(calicam_capture src/main.cpp)

target_link_libraries(
        calicam_capture
        ${OpenCV_LIBS}
        ${catkin_LIBRARIES}
)

cmake_minimum_required (VERSION 2.6)
project (Color_Tracker)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(DEBUG_OPTIONS -g)
set(LANGUAGE_OPTIONS -std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEBUG_OPTIONS} ${LANGUAGE_OPTIONS}")

set(INCLUDE_DIR "../include")
set(SRC_DIR "../src")

file(GLOB_RECURSE HEADERS ../include/*.hpp)
file(GLOB_RECURSE SRC ../src/*.cpp)

include_directories(
    ${INCLUDE_DIR}
)

add_executable(Color_Tracker ${SRC_DIR}/Color_Tracker_New.cpp)
target_link_libraries(Color_Tracker ${OpenCV_LIBS})

CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(CGDI-Voxel-Raycasting)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2")
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -g")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(${PROJECT_SOURCE_DIR}/deps)
message(STATUS "Deps: ${PROJECT_SOURCE_DIR}/deps")

############################################################
# DEPS
include(geometry-central)
include(polyscope)

############################################################
# Executable
add_executable(raycast src/main.cpp src/scene.cpp)
target_link_libraries(raycast polyscope geometry-central)


cmake_minimum_required(VERSION 3.12)
project(Tubes)

# define options
option(BUILD_TESTING "build unit test" ON)

# cpp 17 minimal
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use conan to find depenecies #
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

# Create library to gather game logic in plain cpp
add_library(
  TubesLib STATIC src/utils.cpp src/logic.cpp src/tube.cpp src/ui_helper.cpp
                  src/game.cpp src/level_builder.cpp)
target_include_directories(TubesLib PUBLIC src)
target_link_libraries(TubesLib)

# test library
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

# create  applicatio with Qt implementation
find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/ui.cpp)
target_link_libraries(${PROJECT_NAME} TubesLib Qt5::Widgets)

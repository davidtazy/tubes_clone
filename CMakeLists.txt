cmake_minimum_required(VERSION 3.12)
project(Tubes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)

add_library(TubesLib STATIC src/utils.cpp src/ui.cpp src/logic.cpp src/tube.cpp
                            src/ui_helper.cpp)
target_include_directories(TubesLib PUBLIC src)
target_link_libraries(TubesLib Qt5::Widgets)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} TubesLib)

add_subdirectory(test)

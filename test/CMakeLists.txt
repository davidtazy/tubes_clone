cmake_minimum_required(VERSION 3.9)
project(tests_tubes_clone)

find_package(Catch2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp test_core.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE TubesLib Catch2::Catch2)

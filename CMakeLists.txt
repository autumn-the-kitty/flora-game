cmake_minimum_required(VERSION 3.31.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(flora VERSION 0.0.0)

find_package(raylib 5.5 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

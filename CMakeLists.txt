cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FlAGS "-g")

project(StarkitServoControl)
include_directories(Inc/SktSrvCtl)

add_subdirectory(Deps/starkit-cs-message)

add_executable(Test Src/EchoTest.cpp)
target_link_libraries(Test PUBLIC CsMessage)
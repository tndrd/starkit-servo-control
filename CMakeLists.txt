cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FlAGS "-g")

project(StarkitServoControl)

add_subdirectory(Deps/starkit-cs-message)

add_library(SKSBase Src/SKSBase.cpp)
target_link_libraries(SKSBase PUBLIC CsMessage)
target_include_directories(SKSBase PUBLIC Inc/)

add_library(SKSBaseTest-MsgDump Src/MsgDump.cpp)
target_link_libraries(SKSBaseTest-MsgDump PUBLIC CsMessage)
target_include_directories(SKSBaseTest-MsgDump PUBLIC Inc/)

add_library(SKSBaseTest-EchoIOMock Src/EchoIOMock.cpp)
target_link_libraries(SKSBaseTest-EchoIOMock PUBLIC SKSBaseTest-MsgDump)
target_include_directories(SKSBaseTest-EchoIOMock PUBLIC Inc/)

add_executable(SKSBaseEchoTest Src/EchoTest.cpp)
target_link_libraries(SKSBaseEchoTest PUBLIC SKSBase SKSBaseTest-EchoIOMock)
# CMake configuration for SimplyLinkedList project
project(SimplyLinkedList)
add_executable(SimplyLinkedList main.cpp)

target_compile_features(SimplyLinkedList PUBLIC cxx_std_23)

# CMake configuration for HelloWorld project
project(HelloWorld)
add_executable(HelloWorld main.cpp)

target_compile_features(HelloWorld PUBLIC cxx_std_23)

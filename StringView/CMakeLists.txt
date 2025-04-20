# CMake configuration for StringView project
project(StringView)
add_executable(StringView main.cpp)

target_compile_features(StringView PUBLIC cxx_std_23)

# CMake configuration for LambdasTemplateSyntax project
project(LambdasTemplateSyntax)
add_executable(LambdasTemplateSyntax main.cpp)

target_compile_features(LambdasTemplateSyntax PUBLIC cxx_std_23)

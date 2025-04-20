# CMake configuration for EigenLinSolve project
project(EigenLinSolve)

# find Eigen3
find_package(Eigen3 3.4 REQUIRED NO_MODULES)

# set Eigen include path manually
#set(EIGEN3_INCLUDE_DIR_MANUAL C:/eigen/include/eigen3)

# define the executable
add_executable(EigenLinSolve main.cpp)

# link the include directories to the target
target_link_libraries(EigenLinSolve Eigen3::Eigen)

# CMake configuration for BoostVectorAdd project
project(BoostVectorAdd)
find_package(Boost REQUIRED)  # find the Boost library
include_directories(${Boost_INCLUDE_DIRS})
add_executable(BoostVectorAdd main.cpp)
target_link_libraries(BoostVectorAdd ${Boost_LIBRARIES})

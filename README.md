## HelloWorld
* modern C++20 features (e.g., ranges, views, and structured bindings)
* demonstrating several functional programming techniques (like functors and function chaining)
* manipulating collections (vectors, lists, sets) in efficient and flexible ways using range-based operations

## EigenLinSolve & BoostVectorAdd
* minimal demo

## SimplyLinkedList
* insertion, deletion, iterative and recursive reversal, various sorting algorithms, and merging two linked lists in multiple ways

## LambdasTemplateSyntax
* generic lambdas with template parameters
* type constraints with concepts
* multiple template parameters
* template lambdas can be specialized and passed into fixed-type std::function wrappers (MSVC currently has limitations)

### *Instructions for Windows*
```
# Boost - in main workspace root
git clone --recursive https://github.com/boostorg/boost.git
cd boost
git checkout master
./bootstrap.bat
./b2 headers
./b2 install --prefix=C:/Boost
```
```
# Eigen - in main workspace root
git clone https://gitlab.com/libeigen/eigen.git
cd eigen
git tag
git checkout 3.4.0
mkdir build && cd build
cmake .. -G"Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=C:/eigen
cmake --build . --target INSTALL --config Release
cmake --build . --config Debug
```
```
# project - in main workspace root
cd cpp_sandbox && mkdir build && cd build
cmake .. -G"Visual Studio 17 2022" -DBOOST_ROOT=C:/Boost -A x64 -T host=x64
cmake --build . --config Release --verbose
```

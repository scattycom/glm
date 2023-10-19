# 指定CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称以及语言
project(demo CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加一个可执行文件
add_executable(demo main.cpp)

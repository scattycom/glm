# 指定CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称以及语言
project(demo CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 收集src目录下的所有.cpp文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "third_party/glad/src/glad.c")

# 添加一个可执行文件
add_executable(demo ${SOURCES})

find_package(Vulkan REQUIRED)
target_include_directories(demo PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(demo PRIVATE ${Vulkan_LIBRARIES})

add_subdirectory(third_party/glfw)

# 链接第三方库
target_link_libraries(demo PRIVATE  glfw)

include_directories(third_party/glad/include)

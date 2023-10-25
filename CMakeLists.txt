# 指定CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称以及语言
project(demo CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 收集所有的shader文件
file(GLOB_RECURSE src "src/cpp/*.cpp")
file(GLOB_RECURSE shader_list "src/shaders/*.spv")
file(GLOB_RECURSE headers "src/header/*.h")

# 添加shader文件到SOURCES列表
list(APPEND SOURCES ${shader_list})
list(APPEND SOURCES ${headers})
list(APPEND SOURCES ${src})
list(APPEND SOURCES "third_party/glad/src/glad.c")

# 在Visual Studio中创建筛选器
source_group("third_party" FILES "third_party/glad/src/glad.c")
source_group("shaders" FILES ${shader_list})
source_group("headers" FILES ${headers})
source_group("src" FILES ${src})

# 添加一个可执行文件
add_executable(demo ${SOURCES})
#设置工作目录
set_target_properties(demo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/Debug")
include_directories(${CMAKE_SOURCE_DIR})

find_package(Vulkan REQUIRED)
target_include_directories(demo PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(demo PRIVATE ${Vulkan_LIBRARIES})

add_subdirectory(third_party/glfw)

# 链接第三方库
target_link_libraries(demo PRIVATE  glfw)

include_directories(third_party/glad/include)

target_compile_definitions(demo PRIVATE ACTIVE=1)

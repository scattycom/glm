# 指定CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称以及语言
project(demo CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 收集所有文件
file(GLOB_RECURSE src "src/cpp/*.cpp")
file(GLOB_RECURSE core "src/core/*.cpp")
file(GLOB_RECURSE headers "src/header/*.h")
file(GLOB_RECURSE shader_list "src/shaders/*.vert" "src/shaders/*.frag")

# 添加文件到SOURCES列表
list(APPEND SOURCES ${shader_list})
list(APPEND SOURCES ${headers})
list(APPEND SOURCES ${src})
list(APPEND SOURCES ${core})
list(APPEND SOURCES "third_party/glad/src/glad.c")

# 在Visual Studio中创建筛选器
source_group("third_party" FILES "third_party/glad/src/glad.c")
source_group("shaders" FILES ${shader_list})
source_group("headers" FILES ${headers})
source_group("core" FILES ${core})
source_group("src" FILES ${src})

# 添加一个可执行文件
add_executable(demo ${SOURCES})
#设置工作目录
set_target_properties(demo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/Debug")
include_directories(${CMAKE_SOURCE_DIR})

#设置Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(demo PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(demo PRIVATE ${Vulkan_LIBRARIES})
# 获取Vulkan库文件路径
get_filename_component(VULKAN_LIB_DIR ${Vulkan_LIBRARY} DIRECTORY)
# 假设bin目录和lib目录是同级的
get_filename_component(VULKAN_ROOT_DIR ${VULKAN_LIB_DIR} DIRECTORY)
set(VULKAN_BIN_DIR "${VULKAN_ROOT_DIR}/bin")
# 将bin目录路径写入文件
file(WRITE "vulkan_bin_path.txt" "${VULKAN_BIN_DIR}")


# 链接第三方库
#glfw
add_subdirectory(third_party/glfw)
target_link_libraries(demo PRIVATE glfw)
set_property(TARGET glfw PROPERTY FOLDER "ThirdPartyLibs")
#box2d
add_subdirectory(third_party/box2d)
target_link_libraries(demo PRIVATE box2d)
set_property(TARGET box2d PROPERTY FOLDER "ThirdPartyLibs")

target_include_directories(demo PRIVATE third_party/glm)
target_include_directories(demo PRIVATE third_party/glad/include)

target_compile_definitions(demo PRIVATE ACTIVE=1)

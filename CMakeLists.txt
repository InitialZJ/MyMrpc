# 设置cmake 最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
project(mrpc)

# 生成debug版本,可以进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")
# ·作用是在生成目录，产生一个compile_commands.json文件\

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(MUDUO_PATH "/home/lzj/build/release-install-cpp11")
find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})
message(STATUS ${Muduo_INCLUDE_DIR})
message(STATUS ${Muduo_LIBRARY_DIR})

include_directories(${Muduo_INCLUDE_DIR})
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)
message(STATUS ${muduo_base})
message(STATUS ${muduo_net})

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIR})

include_directories(/usr/local/include/zookeeper)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置项目编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
# 设置项目库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
# 继续去example 中找cmakelist
add_subdirectory(example) 
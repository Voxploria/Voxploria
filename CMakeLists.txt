cmake_minimum_required(VERSION 3.20)
project(IDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/stb)

file(GLOB SOURCES "src/*.cpp")

add_executable(Voxploria ${SOURCES})

target_link_libraries(Voxploria ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})

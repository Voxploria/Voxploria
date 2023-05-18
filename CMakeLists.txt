cmake_minimum_required(VERSION 3.19)
project(Voxploria)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Add your source files here
file(GLOB_RECURSE VOXPLORIA_SOURCES "src/*.cpp")
file(GLOB_RECURSE VOXPLORIA_HEADERS "include/*.h")

# Create the executable
add_executable(Voxploria ${VOXPLORIA_SOURCES} ${VOXPLORIA_HEADERS})

# Link against SDL2 and Vulkan libraries
target_link_libraries(Voxploria ${SDL2_LIBRARIES} vulkan)

# Set the include directories
target_include_directories(Voxploria PRIVATE "include")

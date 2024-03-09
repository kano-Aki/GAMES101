cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include ./include)

add_executable(Rasterizer src/main.cpp src/rasterizer.hpp src/rasterizer.cpp src/global.hpp src/Triangle.hpp src/Triangle.cpp src/Texture.hpp src/Texture.cpp src/Shader.hpp src/OBJ_Loader.h)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)
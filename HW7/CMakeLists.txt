cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing src/main.cpp src/Object.hpp src/Vector.cpp src/Vector.hpp src/Sphere.hpp src/global.hpp src/Triangle.hpp src/Scene.cpp
        src/Scene.hpp src/Light.hpp src/AreaLight.hpp src/BVH.cpp src/BVH.hpp src/Bounds3.hpp src/Ray.hpp src/Material.hpp src/Intersection.hpp
        src/Renderer.cpp src/Renderer.hpp)

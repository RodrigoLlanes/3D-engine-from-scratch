cmake_minimum_required(VERSION 3.26)
project(graphic_engine)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

include_directories(
        include
)

file(GLOB src
        "src/*.cpp"
        "src/shaders/*.cpp"
)

add_executable(${PROJECT_NAME} ${src}
        include/vectors.hpp)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
)

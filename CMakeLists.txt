cmake_minimum_required(VERSION 3.15)
project(engine CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

include_directories(
        build/include 
        include
        ${SDL2_INCLUDE_DIRS}
)

file(GLOB engine_src 
        "src/utils/*.cpp"
        "src/*.cpp"
        "src/*.c"
)

add_executable(${PROJECT_NAME} ${engine_src})
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
)

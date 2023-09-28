cmake_minimum_required(VERSION 3.26)
project(graphic_engine)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

include_directories(
        include
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB src
        "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        Eigen3::Eigen
)

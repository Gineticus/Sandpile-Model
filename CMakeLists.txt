cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(
    labwork3
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(lib/args)
add_subdirectory(lib/sandpile)
add_subdirectory(lib/array)
add_subdirectory(lib/bmp)

target_include_directories(${PROJECT_NAME} PUBLIC lib/args)
target_link_libraries(${PROJECT_NAME} args)

target_include_directories(${PROJECT_NAME} PUBLIC lib/sandpile)
target_link_libraries(${PROJECT_NAME} sandpile)

target_include_directories(${PROJECT_NAME} PUBLIC lib/array)
target_link_libraries(${PROJECT_NAME} array)

target_include_directories(${PROJECT_NAME} PUBLIC lib/bmp)
target_link_libraries(${PROJECT_NAME} bmp)
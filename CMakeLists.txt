cmake_minimum_required(VERSION 3.10)
project(imu LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(imu STATIC
    src/daemon.c
    src/mpu6050.c
)

target_include_directories(imu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(imu PRIVATE pigpiod_if2 pthread rt)

target_compile_features(imu PRIVATE c_std_11)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()

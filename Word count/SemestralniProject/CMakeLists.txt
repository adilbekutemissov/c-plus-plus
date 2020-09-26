cmake_minimum_required(VERSION 3.10.2)
project(SemestralniProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(SemestralniProject main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SemestralniProject Threads::Threads)

file(GLOB SemestralniProject-FILES ${CMAKE_SOURCE_DIR}/files/*)
add_custom_command(TARGET SemestralniProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/files $<TARGET_FILE_DIR:SemestralniProject>/files)
cmake_minimum_required(VERSION 3.15)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main
    main.cpp ${SOURCES}
)
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

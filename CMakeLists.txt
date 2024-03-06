cmake_minimum_required(VERSION 3.23)
project(cpp_demo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -g")

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

qt_add_executable(cpp_demo
    main.cpp
)

target_link_libraries(cpp_demo PRIVATE Qt6::Core)
target_compile_options(cpp_demo PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
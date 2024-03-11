cmake_minimum_required(VERSION 3.23)
project(cpp_demo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -g")

find_package(Curses REQUIRED)
add_executable(cpp_demo main.cpp gameobject.cpp utils.cpp)
target_link_libraries(cpp_demo PRIVATE ${CURSES_LIBRARIES})
target_compile_options(cpp_demo PUBLIC -Wall -Wextra -pedantic -pedantic-errors)
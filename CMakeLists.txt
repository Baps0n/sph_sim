cmake_minimum_required(VERSION 3.20)

project(sph_sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics window system)
add_executable(sph_sim src/main.cpp)
target_link_libraries(sph_sim PUBLIC sfml-graphics sfml-window)

enable_testing()
add_subdirectory(external/doctest)

add_executable(sph_test src/sph_test.cpp)
target_link_libraries(sph_test PUBLIC sfml-graphics sfml-window doctest)

cmake_minimum_required(VERSION 3.6)

project(bengi_cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(thirdparty/catch)
add_subdirectory(thirdparty/cxxopts)
add_subdirectory(src/vm)
add_subdirectory(src/assembler)
add_subdirectory(src/tests)


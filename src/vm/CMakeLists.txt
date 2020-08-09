project(bengi)

add_library(bengi_lib STATIC bengi/vm.cpp 
                             bengi/vm.h
                             bengi/bytecode.cpp 
                             bengi/bytecode.h
                             bengi/version.h)

target_include_directories(bengi_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bengi bengi/main.cpp)
target_link_libraries(bengi PRIVATE bengi_lib basm_lib cxxopts)

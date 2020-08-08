project(basm)

add_library(basm_lib STATIC  basm/assembler.cpp 
                             basm/assembler.h
                             basm/lexer.cpp 
                             basm/lexer.h
                             )
target_link_libraries(basm_lib bengi_lib)
target_include_directories(basm_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(basm basm/main.cpp)
target_link_libraries(basm PRIVATE basm_lib cxxopts)
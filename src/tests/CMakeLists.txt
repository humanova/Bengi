project(bengi_tests)

add_executable(tests testmain.cpp)
target_link_libraries(tests PRIVATE bengi_lib basm_lib catch)
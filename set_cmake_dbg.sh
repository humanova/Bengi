rm -r build
mkdir build && cd build && cmake -D CMAKE_C_COMPILER=/usr/bin/gcc -D CMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cd .. && python3 misc/scripts/gen_test_header.py
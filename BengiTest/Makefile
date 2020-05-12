CFLAGS = -std=c++11

ifeq ($(OS), Windows_NT)
	BIN = bin/Win32
else
	BIN = bin/Linux
endif

BUILD_DIR  = ../$(BIN)

run_tests: VM.o main.o
	g++ $(CFLAGS) -o $(BIN)/run_tests $(BIN)/VM.o $(BIN)/main.o
	cp -f $(BIN)/run_tests $(BUILD_DIR)

main.o:
	g++ $(CFLAGS) -c main.cpp -o $(BIN)/main.o

VM.o:
	g++ $(CFLAGS) -c ../Bengi/Bengi/VM.cpp -o $(BIN)/VM.o

clean:
	rm $(BIN)/*.o
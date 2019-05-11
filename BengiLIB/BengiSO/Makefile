CFLAGS = -std=c++11 -shared -fPIC

ifeq ($(OS), Windows NT)
	BIN = bin/Win32
else
	BIN = bin/Linux
endif

BUILD_DIR  = ../../$(BIN)

bengi: Error.o IOFile.o VM.o Bengi.o
	g++ $(CFLAGS) -o $(BIN)/bengi.so $(BIN)/VM.o $(BIN)/Error.o $(BIN)/IOFile.o $(BIN)/Bengi.o
	cp -f $(BIN)/bengi.so $(BUILD_DIR)

Bengi.o:
	g++ $(CFLAGS) -c Bengi.cc -o $(BIN)/Bengi.o

VM.o:
	g++ $(CFLAGS) -c ../../Bengi/Bengi/VM.cpp -o $(BIN)/VM.o

IOFile.o:
	g++ $(CFLAGS) -c ../../Bengi/Bengi/IOFile.cpp -o $(BIN)/IOFile.o

Error.o:
	g++ $(CFLAGS) -c ../../Bengi/Bengi/Error.cpp -o $(BIN)/Error.o

clean:
	rm $(BIN)/*.o
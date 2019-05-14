
CFLAGS = -std=c++11
SRC_DIR = Bengi/

ifeq ($(OS), Windows_NT)
	BIN = bin/Win32
else
	BIN = bin/Linux
endif

BUILD_DIR  = ../$(BIN)

bengi: Error.o Version.o Usage.o ArgParser.o IOFile.o main.o VM.o 
	g++ $(CFLAGS) -o $(BIN)/bengi $(BIN)/Error.o $(BIN)/Version.o $(BIN)/Usage.o $(BIN)/ArgParser.o $(BIN)/IOFile.o $(BIN)/main.o $(BIN)/VM.o
	cp -f $(BIN)/bengi $(BUILD_DIR)

# no difference except output name (change DEBUG macro to 1 in VM.h)
bengi_dbg: Error.o Version.o Usage.o ArgParser.o IOFile.o main.o VM.o 
	g++ $(CFLAGS) -o $(BIN)/bengi_dbg $(BIN)/Error.o $(BIN)/Version.o $(BIN)/Usage.o $(BIN)/ArgParser.o $(BIN)/IOFile.o $(BIN)/main.o $(BIN)/VM.o
	cp -f $(BIN)/bengi_dbg $(BUILD_DIR)

main.o: $(SRC_DIR)/main.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $(BIN)/main.o

IOFile.o: $(SRC_DIR)/IOFile.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/IOFile.cpp -o $(BIN)/IOFile.o

Error.o: $(SRC_DIR)/Error.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/Error.cpp -o $(BIN)/Error.o

Version.o: $(SRC_DIR)/Version.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/Version.cpp -o $(BIN)/Version.o

Usage.o: $(SRC_DIR)/Usage.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/Usage.cpp -o $(BIN)/Usage.o

ArgParser.o: $(SRC_DIR)/ArgParser.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/ArgParser.cpp -o $(BIN)/ArgParser.o

VM.o: $(SRC_DIR)/VM.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/VM.cpp -o $(BIN)/VM.o

clean:
	rm $(BIN)/*.o
	

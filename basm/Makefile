
CFLAGS = -std=c++11

SRC_DIR = basm/

ifeq ($(OS), Windows NT)
	BIN = bin/Win32
else
	BIN = bin/Linux
endif

BUILD_DIR  = ../$(BIN)

basm: Lexer.o benasm.o
	g++ $(CFLAGS) -o $(BIN)/basm $(BIN)/Lexer.o $(BIN)/benasm.o 
	cp -f $(BIN)/basm $(BUILD_DIR)

benasm.o: $(SRC_DIR)/benasm.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/benasm.cpp -o $(BIN)/benasm.o

Lexer.o: $(SRC_DIR)/Lexer.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/Lexer.cpp -o $(BIN)/Lexer.o

clean:
	rm $(BIN)/*.o
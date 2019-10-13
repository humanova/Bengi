
CFLAGS = -std=c++11

SRC_DIR = basm/

ifeq ($(OS), Windows_NT)
	BIN = bin/Win32
else
	BIN = bin/Linux
endif

BUILD_DIR  = ../$(BIN)

basm: Lexer.o main.o
	g++ $(CFLAGS) -o $(BIN)/basm $(BIN)/Lexer.o $(BIN)/main.o 
	cp -f $(BIN)/basm $(BUILD_DIR)

main.o: $(SRC_DIR)/main.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $(BIN)/main.o

Lexer.o: $(SRC_DIR)/Lexer.cpp
	g++ $(CFLAGS) -c $(SRC_DIR)/Lexer.cpp -o $(BIN)/Lexer.o

clean:
	rm $(BIN)/*.o
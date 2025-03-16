CC = g++
FLAGS = -Wall -Werror -Wextra -Wpedantic

all: lexer

rebuild: clean all

lexer:
	$(CC) $(FLAGS) lexer.cpp -o lexer
	./lexer

clean:
	rm -rf lexer
all:
	gcc -Wall -Werror -Wextra -Wpedantic -Wshadow -Wstrict-prototypes -o cosh src/*.c -lgmp

clean:
	rm -f cosh

format:
	clang-format -i src/*

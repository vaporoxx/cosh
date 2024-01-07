all:
	gcc -Wall -Werror -Wextra -Wpedantic -Wshadow -Wstrict-prototypes -o cosh src/*.c

clean:
	rm -f cosh

format:
	clang-format -i src/*

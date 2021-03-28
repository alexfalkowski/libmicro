CFLAGS=-I/usr/local/opt/openssl/include -Wall -Wextra -Werror -pedantic -O3
SRC=$(shell find . -name "*.c")

clang-format:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=google --dry-run --Werror

cppcheck:
	cppcheck --platform=unix64 --error-exitcode=1 --enable=all --suppress=missingInclude .

lint: clang-format cppcheck

format:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=google --Werror -i

verify: lint

compile:
	gcc $(CFLAGS) -o libmicro.o -c $(SRC)

link:
	ar -cq libmicro.a *.o
clean:
	rm -f *.o *.a

lib: clean compile link

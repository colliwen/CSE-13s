CC = clang
CFLAGS = -g3 -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm
TESTS = tests
EXEC = calc
HEADERS = mathlib.h messages.h stack.h operators.h

all: $(TESTS) $(EXEC)

$(TESTS): $(TESTS).o mathlib.o stack.o operators.o
	$(CC) $^ $(LFLAGS) -o $@

$(EXEC): $(EXEC).o mathlib.o stack.o operators.o
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(TESTS) *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format


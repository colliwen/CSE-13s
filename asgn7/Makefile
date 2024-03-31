CC=clang
debug:   CFLAGS=-Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -Werror -pedantic
release: CFLAGS=-Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm

EXECBIN=xd


.PHONY: all debug release clean

all: debug

debug: $(EXECBIN)

release: $(EXECBIN)


%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN)

format:
	clang-format -i --style=file *.[ch]




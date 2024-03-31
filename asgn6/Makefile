CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -Wconversion -Wdouble-promotion -pedantic -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -Wconversion -Wdouble-promotion -pedantic
LDFLAGS=-lm

EXECBIN=colorb
EXECBIN2=iotest

    
OBJS=\
    bmp.o \
    io.o \
    colorb.o \

OBJS2=\
    bmp.o\
    io.o\
    colorb.o\

.PHONY: all debug release clean

all: debug

debug: $(EXECBIN) $(EXECBIN2)

release: $(EXECBIN) $(EXECBIN2)

$(EXECBIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(EXECBIN2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $^


%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(EXECBIN2) $(OBJS) $(OBJS2)

format:
	clang-format -i --style=file *.[ch]




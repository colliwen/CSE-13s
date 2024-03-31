CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -Wconversion -Wdouble-promotion -pedantic -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -Wconversion -Wdouble-promotion -pedantic
LDFLAGS=-lm

huffBIN=huff
dehuffBIN=dehuff
bwtestBIN=bwtest
brtestBIN=brtest
nodetestBIN=nodetest
pqtestBIN=pqtest

    
OBJS=\
     bitwriter.o\
     bitreader.o\
     node.o\
     pq.o\


.PHONY: all debug release clean

all: debug

debug: $(huffBIN) $(dehuffBIN) $(bwtestBIN) $(brtestBIN) $(nodetestBIN) $(pqtestBIN)

release: $(huffBIN) $(dehuffBIN) $(bwtestBIN) $(brtestBIN) $(nodetestBIN) $(pqtestBIN)

$(huffBIN): $(OBJS) huff.o
	$(CC) $(LDFLAGS) -o $@ $^

$(dehuffBIN): $(OBJS) dehuff.o
	$(CC) $(LDFLAGS) -o $@ $^

$(bwtestBIN): bitwriter.o bwtest.o
	$(CC) $(LDFLAGS) -o $@ $^

$(brtestBIN): bitreader.o brtest.o
	$(CC) $(LDFLAGS) -o $@ $^

$(nodetestBIN): node.o nodetest.o
	$(CC) $(LDFLAGS) -o $@ $^

$(pqtestBIN): pq.o node.o pqtest.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(huffBIN) $(dehuffBIN) $(bwtestBIN) $(brtestBIN) $(nodetestBIN) $(pqtestBIN) $(OBJS) huff.o dehuff.o bwtest.o brtest.o nodetest.o pqtest.o

format:
	clang-format -i --style=file *.[ch]


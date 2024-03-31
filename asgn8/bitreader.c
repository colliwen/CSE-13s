#include "bitreader.h"

#include <stdio.h>
#include <stdlib.h>

struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitReader *bit_read_open(const char *filename) {
    /*
2 allocate a new BitReader
3 open the filename for reading as a binary file, storing the result in FILE *f
4 store f in the BitReader field underlying_stream
5 clear the byte field of the BitReader to 0
6 set the bit_position field of the BitReader to 8
7 if any step above causes an error:
8 return NULL
9 else:
10 return a pointer to the new BitReader
	*/
    BitReader *reader = calloc(1, sizeof(BitReader));
    if (reader == NULL) {
        return NULL;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }
    reader->underlying_stream = f;
    reader->byte = 0;
    reader->bit_position = 8;
    if (reader == NULL) {
        return NULL;
    }
    return reader;
}

void bit_read_close(BitReader **pbuf) {
    /*
2 if *pbuf != NULL:
3 close the underlying_stream
4 free *pbuf
5 *pbuf = NULL
6 if any step above causes an error:
7 report fatal error
	*/
    if (*pbuf != NULL) {
        if (fclose((*pbuf)->underlying_stream) == EOF) {
            fprintf(stderr, "error closing reader file");
            exit(1);
        }
        free(*pbuf);
        *pbuf = NULL;
    }
}

uint8_t bit_read_bit(BitReader *buf) {
    /*
2 if bit_position > 7:
3 read a byte from the underlying_stream using fgetc()
4 bit_position = 0
5 get the bit numbered bit_position from byte
6 bit_position += 1;
7 if any step above causes an error:
8 report fatal error
9 else:
10 return the bit
	*/
    if (buf == NULL) {
        fprintf(stderr, "bit_read_bit error, buf == NULL");
        exit(1);
    }
    if (buf->bit_position > 7) {
        int got = fgetc(buf->underlying_stream);
        if (got == EOF) {
            fprintf(stderr, "error bit_read_bit reading bit");
            exit(1);
        }
        buf->byte = (uint8_t) got;
        buf->bit_position = 0;
    }
    uint8_t bit = (buf->byte >> buf->bit_position) & 1;
    buf->bit_position += 1;
    return bit;
}

uint8_t bit_read_uint8(BitReader *buf) {
    /*
2 uint8_t byte = 0x00
3 for i in range(0, 8):
4 read a bit b from the underlying_stream
5 set bit i of byte to the value of b
6 return byte
	*/
    uint8_t byte = 0x00;
    for (int i = 0; i < 8; i++) {
        uint8_t b = bit_read_bit(buf);
        byte = (uint8_t) (b << i) | byte;
    }
    return byte;
}

uint16_t bit_read_uint16(BitReader *buf) {
    /*
2 uint16_t word = 0x0000
3 for i in range(0, 16):
4 read a bit b from the underlying_stream
5 set bit i of word to the value of b
6 return word;
	*/
    uint16_t word = 0x0000;
    for (int i = 0; i < 16; i++) {
        uint8_t b = bit_read_bit(buf);
        word = (uint16_t) (b << i) | word;
    }
    return word;
}

uint32_t bit_read_uint32(BitReader *buf) {
    /*
2 uint32_t word = 0x00000000
3 for i in range(0, 32):
4 read a bit b from the underlying_stream
5 set bit i of word to the value of b
6 return word;
	*/
    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        uint8_t b = bit_read_bit(buf);
        word = (uint32_t) (b << i) | word;
    }
    return word;
}

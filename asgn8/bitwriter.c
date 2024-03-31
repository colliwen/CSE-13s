#include "bitwriter.h"

#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
} Bitwriter;

BitWriter *bit_write_open(const char *filename) {
    /*
2 allocate a new BitWriter
3 open the filename for writing as a binary file, storing the result in FILE *f
4 store f in the BitWriter field underlying_stream
5 clear the byte and bit_positions fields of the BitWriter to 0
6 if any step above causes an error:
7 return NULL
8 else:
9 return a pointer to the new BitWriter
	*/

    BitWriter *writer = calloc(1, sizeof(Bitwriter));
    if (writer == NULL) {
        return NULL;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return NULL;
    }
    writer->underlying_stream = f;
    writer->byte = 0;
    writer->bit_position = 0;
    if (writer == NULL) {
        return NULL;
    }
    return writer;
}

void bit_write_close(BitWriter **pbuf) {
    /*
2 if *pbuf != NULL:
3 if (*pbuf)->bit_position > 0:
4 //(*pbuf)->byte contains at least one bit that has not yet been written
5 write the byte to the underlying_stream using fputc()
6 close the underlying_stream
7 free the BitWriter
8 *pbuf = NULL
	*/
    if (*pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) {
            if (fputc((*pbuf)->byte, (*pbuf)->underlying_stream) == EOF) {
                fprintf(stderr, "error writing byte to file in bit_write_close()");
                exit(1);
            }
        }
        if (fclose((*pbuf)->underlying_stream) == EOF) {
            fprintf(stderr, "error closing file");
            exit(1);
        }
        free(*pbuf);
        *pbuf = NULL;
    }
}

void bit_write_bit(BitWriter *buf, uint8_t bit) {
    /*
2 if bit_position > 7:
3 write the byte to the underlying_stream using fputc()
4 clear the byte and bit_position fields of the BitWriter to 0
5 set the bit at bit_position of the byte to the value of bit
6 bit_position += 1
	*/
    if (buf == NULL) {
        fprintf(stderr, "bit_write_bit error, buf == NULL");
        exit(1);
    }
    if ((*buf).bit_position > 7) {
        if (fputc(buf->byte, buf->underlying_stream) == EOF) {
            fprintf(stderr, "error bit_write_bit writing bit");
            exit(1);
        }
        buf->byte = 0;
        buf->bit_position = 0;
    }
    buf->byte |= bit << buf->bit_position;
    buf->bit_position += 1;
}

void bit_write_uint8(BitWriter *buf, uint8_t x) {
    /*
2 for i = 0 to 7:
3 write bit i of x using bit_write_bit()
	*/
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (x >> i) & 0x01;
        bit_write_bit(buf, bit);
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    /*
2 for i = 0 to 15:
3 write bit i of x using bit_write_bit()
	*/
    for (int i = 0; i < 16; i++) {
        uint8_t bit = (x >> i) & 0x01;
        bit_write_bit(buf, bit);
    }
}

void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        uint8_t bit = (x >> i) & 0x01;
        bit_write_bit(buf, bit);
    }
}

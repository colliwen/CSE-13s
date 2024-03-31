#include "io.h"

#include <stdlib.h>

void read_uint8(FILE *fin, uint8_t *px) {
    int result = fgetc(fin);
    if (result == EOF) {
        fprintf(stderr, "read_uint8 result == EOF");
        exit(1);
    }
    *px = (uint8_t) result;
}

void read_uint16(FILE *fin, uint16_t *px) {
    uint8_t a = 0;
    uint8_t b = 0;
    uint16_t new = 0;
    read_uint8(fin, &a);
    read_uint8(fin, &b);
    new = (uint16_t) b;
    new = (uint16_t) (new << 8);
    *px = new | (uint16_t) a;
}

void read_uint32(FILE *fin, uint32_t *px) {
    uint16_t a = 0;
    uint16_t b = 0;
    uint32_t new = 0;
    read_uint16(fin, &a);
    read_uint16(fin, &b);
    new = ((new | (uint32_t) b)) << 16;
    *px = new | (uint32_t) a;
}

void write_uint8(FILE *fout, uint8_t x) {
    int result = fputc(x, fout);
    if (result == EOF) {
        fprintf(stderr, "write_uint8 result == EOF");
        exit(1);
    }
}

void write_uint16(FILE *fout, uint16_t x) {
    write_uint8(fout, (uint8_t) x);
    write_uint8(fout, (uint8_t) (x >> 8));
}

void write_uint32(FILE *fout, uint32_t x) {
    write_uint16(fout, (uint16_t) x);
    write_uint16(fout, (uint16_t) (x >> 16));
}

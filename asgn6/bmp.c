#include "bmp.h"

#include "io.h"

#include <math.h>
#include <stdlib.h>

#define MAX_COLORS 256

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

typedef struct bmp {
    uint32_t height;
    uint32_t width;
    Color palette[MAX_COLORS];
    uint8_t **a;
} BMP;

uint32_t round_up(uint32_t x, uint32_t n) {
    while (x % n != 0) {
        x = x + 1;
    }
    return x;
}

BMP *bmp_create(FILE *fin) {
    BMP *pbmp = calloc(1, sizeof(BMP));
    if (pbmp == NULL) {
        fprintf(stderr, "pmbp == NULL");
        exit(1);
    }

    uint8_t type1;
    uint8_t type2;

    // read data from the input file
    read_uint8(fin, &type1);
    read_uint8(fin, &type2);

    uint8_t skip8;
    uint16_t skip16;
    uint32_t skip32;
    /*
    skip uint32 (that is, read a uint32 into a variable, but you won't use the value)
    skip uint16
    skip uint16
    skip uint32
    */
    read_uint32(fin, &skip32);
    read_uint16(fin, &skip16);
    read_uint16(fin, &skip16);
    read_uint32(fin, &skip32);

    uint32_t bitmap_header_size;
    read_uint32(fin, &bitmap_header_size);
    read_uint32(fin, &pbmp->width);
    read_uint32(fin, &pbmp->height);

    //skip uint16
    read_uint16(fin, &skip16);

    uint16_t bits_per_pixel;
    uint32_t compression;
    read_uint16(fin, &bits_per_pixel);
    read_uint32(fin, &compression);

    /*
    skip uint32
    skip uint32
    skip uint32
    */
    read_uint32(fin, &skip32);
    read_uint32(fin, &skip32);
    read_uint32(fin, &skip32);

    uint32_t colors_used;
    read_uint32(fin, &colors_used);

    //skip uint32
    read_uint32(fin, &skip32);

    /*
    verify type1 == 'B'
    verify type2 == 'M'
    verify bitmap_header_size == 40
    verify bits_per_pixel == 8
    verify compression == 0
    */
    if (type1 != 'B') {
        fprintf(stderr, "type != 'B', = %c", type1);
        exit(1);
    }
    if (type2 != 'M') {
        fprintf(stderr, "type2 != 'M', = %c", type2);
        exit(1);
    }
    if (bitmap_header_size != 40) {
        fprintf(stderr, "bitmap_header_size != 40, = %d", bitmap_header_size);
        exit(1);
    }
    if (bits_per_pixel != 8) {
        fprintf(stderr, "bits_per_pixel != 8, = %d", bits_per_pixel);
        exit(1);
    }
    if (compression != 0) {
        fprintf(stderr, "compression != 0, = %d", compression);
        exit(1);
    }

    uint32_t num_colors = colors_used;
    if (num_colors == 0) {
        num_colors = (1 << bits_per_pixel);
    }

    for (uint32_t i = 0; i < num_colors; i++) {
        // read bytes from the input file
        read_uint8(fin, &pbmp->palette[i].blue);
        read_uint8(fin, &pbmp->palette[i].green);
        read_uint8(fin, &pbmp->palette[i].red);
        //skip uint8
        read_uint8(fin, &skip8);
    }

    // Each row must have a multiple of 4 pixels.  Round up to next multiple of 4.
    uint32_t rounded_width = round_up(pbmp->width, 4);

    // Allocate pixel array
    pbmp->a = calloc(pbmp->width, sizeof(pbmp->a[0]));
    for (uint32_t x = 0; x < pbmp->width; x++) {
        pbmp->a[x] = calloc(pbmp->height, sizeof(pbmp->a[x][0]));
    }

    // read pixels
    for (uint32_t y = 0; y < pbmp->height; y++) {
        for (uint32_t x = 0; x < pbmp->width; x++) {
            read_uint8(fin, &pbmp->a[x][y]);
        }

        // skip any extra pixels per row
        for (uint32_t x = pbmp->width; x < rounded_width; x++) {
            read_uint8(fin, &skip8);
        }
    }

    return pbmp;
}

void bmp_write(const BMP *pbmp, FILE *fout) {
    uint32_t rounded_width = round_up(pbmp->width, 4);
    uint32_t image_size = pbmp->height * rounded_width;
    uint32_t file_header_size = 14;
    uint32_t bitmap_header_size = 40;
    uint32_t num_colors = MAX_COLORS;
    uint32_t palette_size = 4 * num_colors;
    uint32_t bitmap_offset = file_header_size + bitmap_header_size + palette_size;
    uint32_t file_size = bitmap_offset + image_size;

    // write data to output file
    write_uint8(fout, 'B');
    write_uint8(fout, 'M');
    write_uint32(fout, file_size);
    write_uint16(fout, 0);
    write_uint16(fout, 0);
    write_uint32(fout, bitmap_offset);
    write_uint32(fout, bitmap_header_size);
    write_uint32(fout, pbmp->width);
    write_uint32(fout, pbmp->height);
    write_uint16(fout, 1);
    write_uint16(fout, 8);
    write_uint32(fout, 0);
    write_uint32(fout, image_size);
    write_uint32(fout, 2835);
    write_uint32(fout, 2835);
    write_uint32(fout, num_colors);
    write_uint32(fout, num_colors);

    // write the palette
    for (uint32_t i = 0; i < num_colors; i++) {
        write_uint8(fout, pbmp->palette[i].blue);
        write_uint8(fout, pbmp->palette[i].green);
        write_uint8(fout, pbmp->palette[i].red);
        write_uint8(fout, 0);
    }

    // write the pixels
    for (uint32_t y = 0; y < pbmp->height; y++) {
        for (uint32_t x = 0; x < pbmp->width; x++) {
            write_uint8(fout, pbmp->a[x][y]);
        }
        // if needed, write extra pixels to make a multiple of 4 pixels per row
        for (uint32_t x = pbmp->width; x < rounded_width; x++) {
            write_uint8(fout, 0);
        }
    }
}

void bmp_free(BMP **ppbmp) {
    for (uint32_t i = 0; i < (*ppbmp)->width; i++) {
        free((*ppbmp)->a[i]);
    }
    free((*ppbmp)->a);
    free(*ppbmp);
    *ppbmp = NULL;
}

uint8_t constrain(double x) {
    x = round(x);
    if (x < 0) {
        x = 0;
    }
    if (x > UINT8_MAX) {
        x = UINT8_MAX;
    }
    return (uint8_t) x;
}

void bmp_reduce_palette(BMP *pbmp) {
    for (int i = 0; i < MAX_COLORS; i++) {
        double r = pbmp->palette[i].red;
        double g = pbmp->palette[i].green;
        double b = pbmp->palette[i].blue;

        double SqLe = 0.00999 * r + 0.0664739 * g + 0.7317 * b;
        double SeLq = 0.153384 * r + 0.316624 * g + 0.057134 * b;

        double r_new;
        double g_new;
        double b_new;

        if (SqLe < SeLq) {
            r_new = constrain(0.426331 * r + 0.875102 * g + 0.0801271 * b);
            g_new = constrain(0.281100 * r + 0.571195 * g + -0.0392627 * b);
            b_new = constrain(-0.0177052 * r + 0.0270084 * g + 1.00247 * b);
        } else {
            r_new = constrain(0.758100 * r + 1.45387 * g + -1.48060 * b);
            g_new = constrain(0.118532 * r + 0.287595 * g + 0.725501 * b);
            b_new = constrain(-0.00746579 * r + 0.0448711 * g + 0.954303 * b);
        }

        pbmp->palette[i].red = (uint8_t) r_new;
        pbmp->palette[i].green = (uint8_t) g_new;
        pbmp->palette[i].blue = (uint8_t) b_new;
    }
}

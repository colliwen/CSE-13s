#include "bmp.h"
#include "io.h"

#include <stdlib.h>
#include <unistd.h>

#define OPTIONS ":i:o:h"

int main(int argc, char **argv) {
    int opt;

    char *infile_name = NULL;
    char *outfile_name = NULL;
    char *usage = "Usage: colorb -i infile -o outfile\n\
        colorb -h\n";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ':':
            fprintf(stderr, "colorb:  unknown or poorly formatted option -%c\n%s", optopt, usage);
            return 1;
        case 'i': infile_name = optarg; break;
        case 'o': outfile_name = optarg; break;
        case 'h': printf("%s", usage); return 0;
        default: fprintf(stderr, "%s", usage); return 1;
        }
    }

    if (infile_name == NULL) {
        fprintf(stderr, "colorb:  -i option is required\n%s", usage);
        return 1;
    }
    if (outfile_name == NULL) {
        fprintf(stderr, "colorb:  -o option is required\n%s", usage);
        return 1;
    }

    FILE *infile = fopen(infile_name, "r");
    if (infile == NULL) {
        fprintf(stderr, "colorb:  error reading input file %s\n%s", infile_name, usage);
        return 1;
    }

    FILE *outfile = fopen(outfile_name, "w");

    BMP *pbmp = bmp_create(infile);
    bmp_reduce_palette(pbmp);
    bmp_write(pbmp, outfile);
    fclose(infile);
    fclose(outfile);
    bmp_free(&pbmp);

    return 0;
}

#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// for getopt:
#include <unistd.h>

#define OPTIONS "aishqbr:n:p:H"

int main(int argc, char **argv) {
    Stats stats;
    int opt;
    Set s = 0x00;
    unsigned seed = 13371453;
    int size = 100;
    int printed = 100;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'H':
            printf("Select at least one sort to perform.\n\
SYNOPSIS\n\
   A collection of comparison-based sorting algorithms.\n\
\n\
USAGE\n\
   ./sorting_ref [-Hahbsqi] [-n length] [-p elements] [-r seed]\n\
\n\
OPTIONS\n\
   -H              Display program help and usage.\n\
   -a              Enable all sorts.\n\
   -h              Enable Heap Sort.\n\
   -b              Enable Batcher Sort.\n\
   -s              Enable Shell Sort.\n\
   -q              Enable Quick Sort.\n\
   -i              Enable Insertion Sort.\n\
   -n length       Specify number of array elements (default: 100).\n\
   -p elements     Specify number of elements to print (default: 100).\n\
   -r seed         Specify random seed (default: 13371453).\n");
            return 0;
        case 'a': s = 0x3f; break;
        case 'i': s = set_insert(s, 0); break;
        case 's': s = set_insert(s, 1); break;
        case 'h': s = set_insert(s, 2); break;
        case 'q': s = set_insert(s, 3); break;
        case 'b': s = set_insert(s, 4); break;
        case 'r':
            seed = atoi(optarg);
            if (seed < 1) {
                seed = 13371453;
            }
            break;
        case 'n': size = atoi(optarg); break;
        case 'p':
            printed = atoi(optarg);
            if (printed > size) {
                printed = size;
            }
            break;
        default:
            fprintf(stderr, "Select at least one sort to perform.\n\
SYNOPSIS\n\
   A collection of comparison-based sorting algorithms.\n\
\n\
USAGE\n\
   ./sorting_ref [-Hahbsqi] [-n length] [-p elements] [-r seed]\n\
\n\
OPTIONS\n\
   -H              Display program help and usage.\n\
   -a              Enable all sorts.\n\
   -h              Enable Heap Sort.\n\
   -b              Enable Batcher Sort.\n\
   -s              Enable Shell Sort.\n\
   -q              Enable Quick Sort.\n\
   -i              Enable Insertion Sort.\n\
   -n length       Specify number of array elements (default: 100).\n\
   -p elements     Specify number of elements to print (default: 100).\n\
   -r seed         Specify random seed (default: 13371453).\n");
            return 1;
        }
    }
    if (printed > size) {
        printed = size;
    }
    srandom(seed);
    // Make array
    int *A = calloc(size, sizeof(int));
    assert(A);

    int *A1 = calloc(size, sizeof(int));
    assert(A1);

    int *A2 = calloc(size, sizeof(int));
    assert(A2);

    int *A3 = calloc(size, sizeof(int));
    assert(A3);

    int *A4 = calloc(size, sizeof(int));
    assert(A4);

    for (int i = 0; i < size; i++) {
        uint32_t num = random();
        num = num & 0x3fffffff;
        A[i] = num;
        A1[i] = num;
        A2[i] = num;
        A3[i] = num;
        A4[i] = num;
    }

    if (set_member(s, 0) == true) {
        insertion_sort(&stats, A, size);
        print_stats(&stats, "Insertion Sort", size);
        if (size > 0) {
            for (int i = 0; i < printed; i++) {
                printf("%13d", A[i]);
                if ((i + 1) % 5 == 0 || printed == 1) {
                    printf("\n");
                }
            }
        }
    }

    reset(&stats);

    if (set_member(s, 2) == true) {
        heap_sort(&stats, A1, size);
        print_stats(&stats, "Heap Sort", size);
        if (size > 0) {
            for (int i = 0; i < printed; i++) {
                printf("%13d", A1[i]);
                if ((i + 1) % 5 == 0 || printed == 1) {
                    printf("\n");
                }
            }
        }
    }

    reset(&stats);

    if (set_member(s, 1) == true) {
        shell_sort(&stats, A2, size);
        print_stats(&stats, "Shell Sort", size);
        if (size > 0) {
            for (int i = 0; i < printed; i++) {
                printf("%13d", A2[i]);
                if ((i + 1) % 5 == 0 || printed == 1) {
                    printf("\n");
                }
            }
        }
    }

    reset(&stats);

    if (set_member(s, 3) == true) {
        quick_sort(&stats, A3, size);
        print_stats(&stats, "Quick Sort", size);
        if (size > 0) {
            for (int i = 0; i < printed; i++) {
                printf("%13d", A3[i]);
                if ((i + 1) % 5 == 0 || printed == 1) {
                    printf("\n");
                }
            }
        }
    }

    reset(&stats);

    if (set_member(s, 4) == true) {
        batcher_sort(&stats, A4, size);
        print_stats(&stats, "Batcher Sort", size);
        if (size > 0) {
            for (int i = 0; i < printed; i++) {
                printf("%13d", A4[i]);
                if ((i + 1) % 5 == 0 || printed == 1) {
                    printf("\n");
                }
            }
        }
    }

    free(A);
    free(A1);
    free(A2);
    free(A3);
    free(A4);

    return 0;
}

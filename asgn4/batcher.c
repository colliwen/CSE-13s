#include "batcher.h"

void comparator(Stats *stats, int *A, int x, int y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, int *A, int n) {
    if (n <= 1) {
        return;
    }

    int bits = 0;
    for (int i = n; i > 0;) {
        bits += 1;
        i >>= 1;
    }

    int t = bits;
    int p = 1 << (t - 1);

    while (p > 0) {
        int q = 1 << (t - 1);
        int r = 0;
        int d = p;

        while (d > 0) {
            for (int j = 0; j < n - d; j++) {
                if ((j & p) == r) {
                    comparator(stats, A, j, j + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}

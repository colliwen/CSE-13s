#include "shell.h"

#include "gaps.h"

void shell_sort(Stats *stats, int *A, int n) {
    for (int i = 0; i < GAPS; i++) {
        int gap = gaps[i];
        for (int k = gap; k < n; k++) {
            int j = k;
            int temp = move(stats, A[k]);
            while (j >= gap && cmp(stats, temp, A[j - gap]) == -1) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
}

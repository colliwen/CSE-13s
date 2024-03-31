#include "insert.h"

void insertion_sort(Stats *stats, int *arr, int length) {
    int k = 1;
    while (k < length) {
        int j = k;
        int temp = move(stats, arr[k]);
        while (j >= 1 && cmp(stats, temp, arr[j - 1]) == -1) {
            arr[j] = move(stats, arr[j - 1]);
            j -= 1;
        }
        arr[j] = move(stats, temp);
        k++;
    }
}

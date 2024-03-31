#include "heap.h"

#include <stdbool.h>

int max_child(Stats *stats, int *A, int first, int last) {
    int left = 2 * first + 1;
    int right = 2 * first + 2;
    //if (right <= last && A[right] > A[left]) {
    if (right <= last && cmp(stats, A[right], A[left]) == 1) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, int *A, int first, int last) {
    bool done = false;
    int parent = first;

    // Move the parent down until max-heap condition met
    while (2 * parent + 1 <= last && done == false) {
        // Parent has at least one child
        int largest_child = max_child(stats, A, parent, last);
        //if (A[parent] < A[largest_child]) {
        if (cmp(stats, A[parent], A[largest_child]) == -1) {
            swap(stats, &A[parent], &A[largest_child]);

            /*
			int temp = A[parent];
			A[parent] = A[largest_child];
			A[largest_child] = temp;		
			*/
            parent = largest_child;
        }

        else {
            done = true;
        }
    }
}

void build_heap(Stats *stats, int *A, int first, int last) {
    if (last > 0) {
        for (int parent = (last - 1) / 2; parent >= first; parent--) {
            fix_heap(stats, A, parent, last);
        }
    }
}

void heap_sort(Stats *stats, int *A, int n) {
    int first = 0;
    int last = n - 1;
    build_heap(stats, A, first, last);
    for (int leaf = last; leaf > first; leaf--) {
        swap(stats, &A[first], &A[leaf]);

        /*
		int temp = A[first];
		A[first] = A[leaf];
		A[leaf] = temp;
		*/

        fix_heap(stats, A, first, leaf - 1);
    }
}

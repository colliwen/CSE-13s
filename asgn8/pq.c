#include "pq.h"

#include "node.h"

#include <stdio.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = calloc(1, sizeof(PriorityQueue));
    if (pq == NULL) {
        return NULL;
    }
    pq->list = NULL;

    return pq;
}

void pq_free(PriorityQueue **q) {
    if (*q != NULL) {
        free(*q);
        *q = NULL;
    }
}

bool pq_is_empty(PriorityQueue *q) {
    if (q->list == NULL) {
        return true;
    }
    return false;
}

bool pq_size_is_1(PriorityQueue *q) {
    if (q->list != NULL && q->list->next == NULL) {
        return true;
    }
    return false;
}

bool pq_less_than(ListElement *e1, ListElement *e2) {
    if (e1->tree->weight < e2->tree->weight) {
        return true;
    } else if (e1->tree->weight == e2->tree->weight && e1->tree->symbol < e2->tree->symbol) {
        return true;
    }
    return false;
}

void enqueue(PriorityQueue *q, Node *tree) {
    /*
2 Allocate ListElement new_element
3 set the tree field to the value of the tree function parameter
4 if the queue is empty:
5 point the queue to new_element
6 elif pq_less_than(new_element, q->list):
7 // New element E1 goes before all existing elements of the list 
8 //
9 // Q -----> E2 --> E2 ... 
10 //    ^
11 //    E1 
12 insert new element E1 as the first element of the list
13 else:
14 // Either the new element E3 goes at the end of the list: 
15 //
16 // Q --> E1 --> E2
17 //                ^
18 //                E3
19 // Or the new element E3 goes before an existing element E4:
20 //
21 // Q --> E1 --> E2 -----> E4 ...
22 //                   ^
23 //                   E3
24 // In both cases, we are looking for existing element E2,
25 // and then we put the new element E3 after it.
26 find existing element E2 (either E2->next is NULL or E3 < E2->next)
27 insert new element after E2
	*/
    ListElement *new_element = calloc(1, sizeof(ListElement));
    new_element->tree = tree;
    if (pq_is_empty(q) == true) {
        q->list = new_element;
    } else if (pq_less_than(new_element, q->list)) {
        new_element->next = q->list;
        q->list = new_element;
    } else {
        if (q->list->next == NULL) {
            q->list->next = new_element;
        } else if (pq_less_than(new_element, q->list->next)) {
            new_element->next = q->list->next;
            q->list->next = new_element;
        } else {
            ListElement *tmp = q->list;
            while (tmp != NULL && pq_less_than(tmp, new_element)) {
                if (tmp->next == NULL) {
                    tmp->next = new_element;
                    break;
                }
                if (pq_less_than(new_element, tmp->next)) {
                    new_element->next = tmp->next;
                    tmp->next = new_element;
                }
                tmp = tmp->next;
            }
        }
    }
}

Node *dequeue(PriorityQueue *q) {
    if (pq_is_empty(q) == true) {
        fprintf(stderr, "dequeue error, queue empty");
        exit(1);
    }
    ListElement *tmp = q->list;
    q->list = q->list->next;
    return tmp->tree;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree);
        e = e->next;
    }
    printf("=============================================\n");
}

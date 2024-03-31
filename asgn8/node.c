#include "node.h"

#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint32_t weight) {
    /*
2 allocate a new Node
3 set the symbol and weight fields of Node to function parameters symbol and weight
4 if any step above causes an error:
5 return NULL
6 else:
7 return a pointer to the new Node
	*/
    Node *noder = calloc(1, sizeof(Node));
    if (noder == NULL) {
        fprintf(stderr, "*node_create allocation error");
        exit(1);
    }
    noder->symbol = symbol;
    noder->weight = weight;
    return noder;
}

void node_free(Node **pnode) {
    /*
2 if *pnode != NULL:
3 free(*pnode)
4 *pnode = NULL
	*/
    if (*pnode != NULL) {
        node_free(&(*pnode)->left);
        node_free(&(*pnode)->right);
        free(*pnode);
        *pnode = NULL;
    }
}

void node_print_node(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_node(tree->right, '/', indentation + 3);
    printf("%*cweight = %.0d", indentation + 1, ch, tree->weight);

    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }
    printf("\n");
    node_print_node(tree->left, '\\', indentation + 3);
}

void node_print_tree(Node *tree) {
    node_print_node(tree, '<', 2);
}

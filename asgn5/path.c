#include "path.h"

#include "graph.h"
#include "stack.h"

#include <assert.h>
#include <stdlib.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = calloc(1, sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    if (pp != NULL && *pp != NULL) {
        if ((*pp)->vertices) {
            stack_free(&(*pp)->vertices);
        }
        free(*pp);
    }
    if (pp != NULL) {
        *pp = NULL;
    }
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (stack_empty(p->vertices)) {
        stack_push(p->vertices, val);
    } else {
        stack_push(p->vertices, val);
        p->total_weight
            += graph_get_weight(g, stack_size(p->vertices) - 1, stack_size(p->vertices));
    }
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t popped;
    stack_pop(p->vertices, &popped);
    if (stack_size(p->vertices) == 0) {
        p->total_weight = 0;
    } else {
        p->total_weight -= graph_get_weight(g, stack_size(p->vertices), popped);
    }
    return stack_size(p->vertices);
}

void path_copy(Path *dst, const Path *src) {
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *f, const Graph *g) {
    /*
	// Create stack and pop all of p->vertices in stack then pop new stack
	// Make sure to free new stack
	Stack *reverse = stack_create(stack_size(p->vertices) + 1);
	uint32_t popped = 0;
	for (uint32_t i = 0; i < stack_size(p->vertices); i++) {
		assert(stack_pop(p->vertices, &popped) == true);
		assert(stack_push(reverse, popped) == true);
	}
	f = fopen("stdout", "w");
	for (uint32_t i = 0; i < stack_size(reverse); i++) {
		assert(stack_pop(reverse, &popped) == true);
		fprintf(f, "%s/n", graph_get_vertex_name(g, popped));
	}
	fclose(f);
	stack_free(&reverse);
	*/
    stack_print(p->vertices, f, graph_get_names(g));
}

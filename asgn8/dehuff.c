#include "bitreader.h"
#include "bitwriter.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Stack Implementation
bool stack_push(Node *stack[], int *stack_size, Node *element) {
    if (*stack_size == 64) {
        return false;
    }

    *stack_size += 1;
    stack[*stack_size - 1] = element;
    return true;
}

bool stack_pop(Node *stack[], int *stack_size, Node *element) {
    if (*stack_size == 0) {
        return false;
    }

    *element = *stack[*stack_size - 1];
    *stack_size -= 1;
    return true;
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    /*
2 read uint8_t type1 from inbuf
3 read uint8_t type2 from inbuf
4 read uint32_t filesize from inbuf
5 read uint16_t num_leaves from inbuf
6 assert(type1 == 'H')
7 assert(type2 == 'C')
8 num_nodes = 2 * num_leaves - 1
9 Node *node
	*/
    Node *stack[64];
    int stack_size = 0;
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);
    assert(type1 == 'H');
    assert(type2 == 'C');
    uint16_t num_nodes = 2 * num_leaves - 1;
    Node *node;

    /*
10 for i in range(0, num_nodes):
	11 read one bit from inbuf
	12 if bit == 1:
		13 read uint8_t symbol from inbuf
		14 node = node_create(symbol, 0)
	15 else:
		16 node = node_create(0, 0)
		17 node->right = stack_pop()
		18 node->left = stack_pop()
	19 stack_push(node)
20 Node *code_tree = stack_pop()
	*/
    for (uint16_t i = 0; i < num_nodes; i++) {
        uint8_t bit = bit_read_bit(inbuf);
        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        } else {
            node = node_create(0, 0);
            if (stack_pop(stack, &stack_size, node->right) == false) {
                fprintf(stderr, "error popping\n");
                exit(1);
            }
            if (stack_pop(stack, &stack_size, node->left) == false) {
                fprintf(stderr, "error popping\n");
                exit(1);
            }
        }
        if (stack_push(stack, &stack_size, node) == false) {
            fprintf(stderr, "error pushing\n");
            exit(1);
        }
    }
    Node *code_tree = node_create(0, 0);
    if (stack_pop(stack, &stack_size, code_tree) == false) {
        fprintf(stderr, "error popping\n");
        exit(1);
    }

    /*
21 for i in range(0, filesize):
	22 node = code_tree
	23 while true:
		24 read one bit from inbuf
		25 if bit == 0:
			26 node = node->left
		27 else:
			28 node = node->right
		29 if node is a leaf:
			30 break
	31 write uint8 node->symbol to fout
	*/
    for (uint32_t i = 0; i < filesize; i++) {
        node = code_tree;

        while (1) {
            uint8_t bit = bit_read_bit(inbuf);
            if (bit == 0) {
                node = node->left;
            } else {
                node = node->right;
            }
            if (node->left == NULL && node->right == NULL) {
                break;
            }
            fputc(node->symbol, fout);
        }
    }
}

#define OPTIONS ":i:o:h"

int main(int argc, char **argv) {
    int opt;

    char *infile_name = NULL;
    char *outfile_name = NULL;
    char *usage = "Usage: dehuff -i infile -o outfile\n\
       huff -v -i infile -o outfile\n\
       huff -h\n";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ':':
            fprintf(stderr, "dehuff:  unknown or poorly formatted option -%c\n%s", optopt, usage);
            return 1;
        case 'i':
            infile_name = optarg;
            if (infile_name == NULL) {
                fprintf(stderr, "dehuff:  -i option is required\n%s", usage);
                return 1;
            }
            FILE *infile = fopen(infile_name, "r");
            if (infile == NULL) {
                fprintf(stderr, "dehuff:  error reading input file %s\n%s", infile_name, usage);
            }
            fclose(infile);
            break;
        case 'o': outfile_name = optarg; break;
        case 'h': printf("%s", usage); return 0;
        default: fprintf(stderr, "%s", usage); return 1;
        }
    }

    if (outfile_name == NULL) {
        fprintf(stderr, "dehuff:  -o option is required\n%s", usage);
        return 1;
    }

    FILE *outfile = fopen(outfile_name, "w");
    BitReader *inbuf = bit_read_open(infile_name);
    dehuff_decompress_file(outfile, inbuf);
    fclose(outfile);
    bit_read_close(&inbuf);
    return 0;
}

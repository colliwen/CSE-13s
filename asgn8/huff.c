#include "bitreader.h"
#include "bitwriter.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    uint32_t filesize = 0;

    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    histogram[0x00]++;
    histogram[0xff]++;
    int byte = 0;
    while ((byte = fgetc(fin)) != EOF) {
        histogram[byte]++;
        filesize++;
    }

    return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    /*
1. Create and fill a Priority Queue.
Go through the histogram, and create a node for every non-zero histogram entry.
Write the number of nodes added to the Priority Queue to *num_leaves.
Initialize each node with the symbol and weight of its histogram entry.
Put each node in the priority queue.
	*/
    PriorityQueue *pq = pq_create();
    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            Node *entry = node_create((uint8_t) i, (uint32_t) histogram[i]);
            enqueue(pq, entry);
            num_leaves++;
        }
    }

    /*
2. Run the Huffman Coding algorithm.
while Priority Queue has more than one entry
Dequeue into left
Dequeue into right
Create a new node with symbol = 0 and weight = left->weight + right->weight
node->left = left
node->right = right
Enqueue the new node
	*/
    while (pq_size_is_1(pq) != 1) {
        Node *left = dequeue(pq);
        Node *right = dequeue(pq);
        Node *new = node_create(0, (left->weight + right->weight));
        new->left = left;
        new->right = right;
        enqueue(pq, new);
    }

    /*
3. Dequeue the queue’s only entry and return it.
	*/
    return dequeue(pq);
}

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    /*
if node is internal:
	//Recursive calls left and right.
	
	//append a 0 to code and recurse 
	//(don't neeed to append a 0; it's already there)
	fill_code_table(code_table, node->left, code, code_length + 1);

	append a 1 to code and recurse
	code |= (uint64_t) 1 << code_length;
	fill_code_table(code_table, node->right, code, code_length + 1);
	*/
    if (node->right != NULL && node->left != NULL) {
        if (node->left != NULL) {
            fill_code_table(code_table, node->left, code, code_length + 1);
        }
        if (node->right != NULL) {
            code |= (uint64_t) 1 << code_length;
            fill_code_table(code_table, node->right, code, code_length + 1);
        }
    }

    /*
else:
	//Leaf node: store the Huffman Code.
	code_table[node->symbol].code = code;
	code_table[node->symbol].code_length = code_length;	
	*/
    else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    /*
2 if node->left == NULL:
        3 //node is a leaf
        4 write bit 1 to outbuf
        5 write uint8 node->symbol to outbuf
6 else:
        7 //node is internal
        8 huff_write_tree(outbuf, node->left)
        9 huff_write_tree(outbuf, node->right)
        10 write bit 0 to outbuf
        */
    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    /*
2 write uint8_t 'H' to outbuf
3 write uint8_t 'C' to outbuf
4 write uint32_t filesize to outbuf
5 write uint16_t num_leaves to outbuf
6 huff_write_tree(outbuf, code_tree)
7 while true:
	8 b = fgetc(fin)
	9 if b == EOF:
		10 break
	11 code = code_table[b].code
	12 code_length = code_table[b].code_length
	13 for i in range(0, code_length):
		14 write bit (code & 1) to outbuf
		15 code >>= 1
	*/
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    while (1) {
        int b = fgetc(fin);
        if (b == EOF) {
            break;
        }
        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_length;
        for (uint8_t i = 0; i < code_length; i++) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
}

#define OPTIONS ":i:o:h"

int main(int argc, char **argv) {
    int opt;

    char *infile_name = NULL;
    char *outfile_name = NULL;
    char *usage = "Usage: huff -i infile -o outfile\n\
       huff -v -i infile -o outfile\n\
       huff -h\n";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ':':
            fprintf(stderr, "huff:  unknown or poorly formatted option -%c\n%s", optopt, usage);
            return 1;
        case 'i':
            infile_name = optarg;
            if (infile_name == NULL) {
                fprintf(stderr, "huff:  -i option is required\n%s", usage);
                return 1;
            }
            FILE *infile = fopen(infile_name, "r");
            if (infile == NULL) {
                fprintf(stderr, "huff:  error reading input file %s\n%s", infile_name, usage);
            }
            fclose(infile);
            break;
        case 'o': outfile_name = optarg; break;
        case 'h': printf("%s", usage); return 0;
        default: fprintf(stderr, "%s", usage); return 1;
        }
    }

    if (outfile_name == NULL) {
        fprintf(stderr, "huff:  -o option is required\n%s", usage);
        return 1;
    }
    FILE *infile = fopen(infile_name, "r");
    uint32_t *histogram = calloc(256, sizeof(uint32_t));
    uint32_t filesize = fill_histogram(infile, histogram);
    uint16_t num_leaves = 0;
    Node *code_tree = create_tree(histogram, &num_leaves);
    node_print_tree(code_tree);
    Code *code_table = calloc(256, sizeof(Code));
    fill_code_table(code_table, code_tree, 0, 0);
    fseek(infile, 0, SEEK_SET);
    BitWriter *outbuf = bit_write_open(outfile_name);
    huff_compress_file(outbuf, infile, filesize, num_leaves, code_tree, code_table);
    fclose(infile);
    bit_write_close(&outbuf);
    return 0;
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int infile = 0;
    if (argc == 2) {
        infile = open(argv[1], O_RDONLY);
        if (infile < 0) {
            return 1;
        }
    }
    if (argc > 2) {
        return 1;
    }
    int index = 0;
    ssize_t count;
    /*
	while ((count = read(infile, buffer, 16)) > 0) {
		printf("%07x0: ", index);
		for (int i = 0; i < 16; i++) {
			if (count > i) {
				printf("%02x", buffer[i]);
			}
			else {
				printf("  ");
			}
			if (i % 2 == 1) {
				printf(" ");
			}
		}
		printf(" ");
		for (int i = 0; i < (int) strlen(buffer); i++) {
			if (buffer[i] < 32 || buffer[i] > 126) {
				printf(".");
			}
			else {
				printf("%c", buffer[i]);
			}
		}
		index++;
		printf("\n");
	}
	*/

    while (1) {
        char buffer[16];
        count = read(infile, buffer, 16);
        if (count > 0) {
            printf("%07x0: ", index);
            for (int i = 0; i < 16; i++) {
                if (count > i) {
                    printf("%02x", buffer[i]);
                } else {
                    printf("  ");
                }
                if (i % 2 == 1) {
                    printf(" ");
                }
            }
            printf(" ");
            for (int i = 0; i < (int) strlen(buffer); i++) {
                if (buffer[i] < 32 || buffer[i] > 126) {
                    printf(".");
                } else {
                    printf("%c", buffer[i]);
                }
            }
            index++;
            printf("\n");
        }
        if (count < 0) {
            return 1;
        }
        if (count == 0) {
            break;
        }
    }

    /*
	if (count < 0) {
		return 1;
	*/
    close(infile);

    return 0;
}

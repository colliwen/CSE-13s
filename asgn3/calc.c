#include "mathlib.h"
#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <stdio.h>
#include <string.h>

// for getopt:
#include <unistd.h>

#define OPTIONS "hm"

int main(int argc, char **argv) {
    bool isLibm = false;
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': printf("%s", USAGE); return 0;
        case 'm': isLibm = true; break;
        default: printf("%s", USAGE); return 1;
        }
    }

    while (1) {
        char expr[STACK_CAPACITY];
        fprintf(stderr, ">");
        fgets(expr, STACK_CAPACITY, stdin);

        // get rid of \n
        expr[strcspn(expr, "\n")] = '\0';

        // saveptr is a variable that strtok_r will use to track its state as we
        // call it multiple times
        char *saveptr;

        // you can set error to true to stop trying to parse the rest of the expression
        bool error = false;
        // strtok_r splits the input string (expr) on any delimiter character in a
        // sequence (for us, only spaces)
        // it stores its own state in saveptr
        // it returns a pointer to the next token, or NULL if we have processed the entire string
        const char *token = strtok_r(expr, " ", &saveptr);

        // num for parse_double
        double num = 0;

        // op for operator
        char op = '\0';

        // loop until we finish parsing the string or we encounter an error
        while (token != NULL && !error) {
            //process token
            // possibly set error to true
            if ((token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'
                    || token[0] == 's' || token[0] == '%' || token[0] == 'c' || token[0] == 't'
                    || token[0] == 'a' || token[0] == 'r')
                && strlen(token) == 1) {
                op = token[0];
            }

            else if (parse_double(token, &num) == true) {
                if (stack_push(num) == false) {
                    fprintf(stderr, ERROR_NO_SPACE, num);
                    error = true;
                }
                num = 0;
            }

            else if (strlen(token) != 1) {
                fprintf(stderr, ERROR_BAD_STRING, token);
                error = true;
            }

            else {
                fprintf(stderr, ERROR_BAD_CHAR, token[0]);
                error = true;
            }

            // apply operators
            if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%') {
                if (apply_binary_operator(binary_operators[(int) op]) == false) {
                    fprintf(stderr, ERROR_BINARY_OPERATOR);
                    error = true;
                }
            }

            else if ((op == 's' || op == 'c' || op == 't' || op == 'a' || op == 'r')
                     && isLibm == false) {
                if (apply_unary_operator(my_unary_operators[(int) op]) == false) {
                    fprintf(stderr, ERROR_UNARY_OPERATOR);
                    error = true;
                }
            }

            else if ((op == 's' || op == 'c' || op == 't' || op == 'a' || op == 'r')
                     && isLibm == true) {
                if (apply_unary_operator(libm_unary_operators[(int) op]) == false) {
                    fprintf(stderr, ERROR_UNARY_OPERATOR);
                    error = true;
                }
            }

            op = '\0';

            // then, at the very end of your loop...
            // (note that we pass NULL instead of expr to indicate we are still processing the same string)
            token = strtok_r(NULL, " ", &saveptr);
        }
	if(error == true) {
	    	stack_clear();
		continue;
	}
	
	else {
        	stack_print();
        	stack_clear();
        	printf("\n");
	}
    }

    return 0;
}

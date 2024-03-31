#include "hangman_helpers.h"

bool is_lowercase_letter(char c) {
    if ('A' <= c && c <= 'Z')
        return false;
    return true;
}

bool validate_secret(const char *secret) {
    int i = 0;
    int size = 0;
    size = (int) strlen(secret);
    for (i = 0; i < size; i++) {
        if ((secret[i] < 'A' && secret[i] != ' ' && secret[i] != '\'' && secret[i] != '-')
            || (secret[i] > 'Z' && secret[i] < 'a') || secret[i] > 'z') {
            printf("invalid character: '%c'\n", secret[i]);
            return false;
        }
    }
    for (i = 0; i < size; i++) {
        if (is_lowercase_letter(secret[i]) == false) {
            printf("invalid character: '%c'\n", secret[i]);
            return false;
        }
    }
    if (strlen(secret) > 256) {
        printf("the secret phrase is over 256 characters\n");
        return false;
    }
    return true;
}

bool string_contains_character(const char *s, char c) {
    int i = 0;
    int size = (int) strlen(s);
    for (i = 0; i < size; i++) {
        if (s[i] == c) {
            return true;
        }
    }
    return false;
}

char read_letter(void) {
    int character;
    printf("Guess a letter: ");
    character = getchar();
    while (getchar() != '\n')
        ;
    char letter = (char) character;
    /*
	while (letter == '\n') {
		printf("Guess a letter: ");
		character = getchar();
		letter = (char) character;
	}
	*/
    return letter;
}

#include "hangman_helpers.h"

int main(int argc, char **argv) {
    // Check for errors
    // check if secret is given or if more than one word without "" is given
    if (argc != 2) {
        printf("wrong number of arguments\nUsage: ./hangman <secret word or phrase>\nif "
               "the secret is multiple words, you mus quote it\n");
        exit(1);
    }

    // check if secret has valid characters, <= 256 characters and there is only lowercase letters
    char *secret = argv[1];
    bool isSecret = true;
    isSecret = validate_secret(secret);
    if (isSecret == false) {
        printf("the secret phrase must contain only lowercase letters, spaces, hyphens, "
               "and apostrophes\n");
        exit(1);
    }

    // Get size of secret
    int size = 0;
    size = (int) strlen(secret);

    // List for ascii chart
    int i = 0;
    char letters[128];
    for (i = 0; i < 128; i++) {
        letters[i] = (char) i;
    }

    // Put -1 for punctuation already being guessed
    letters[32] = (char) 126;
    letters[39] = (char) 126;
    letters[45] = (char) 126;

    // Number of lives
    int lives = 7;

    int l = 0;

    // Start the process
    while (lives != 0) {
        // Clear screen
        printf("%s", CLEAR_SCREEN);

        // Print gallows
        printf("%s\n\n", arts[7 - lives]);

        // Print phrase
        printf("    Phrase: ");

        int j = 0;
        int g = 0;
        for (i = 0; i < size; i++) {
            for (j = 0; j < 124; j++) {
                if (letters[j] == (char) 126 && secret[i] == (char) j) {
                    printf("%c", secret[i]);
                    g = 1;
                }
            }
            if (g == 0) {
                printf("_");
            }
            g = 0;
        }

        printf("\n");

        // Print eliminated
        printf("Eliminated: ");
        // Go through letters and if letters == 0 that means the letter has been eliminated and print it

        for (i = 0; i < 124; i++) {
            if (letters[i] == '}') {
                printf("%c", i);
            }
        }
        printf("\n\n");
        // Check if win
        for (i = 0; i < size; i++) {
            for (j = 0; j < 124; j++) {
                if (letters[j] == (char) 126 && secret[i] == (char) j) {
                    l += 1;
                }
            }
        }
        if (l == size) {
            printf("You win! The secret phrase was: %s\n", secret);
            return (0);

        } else {
            l = 0;
        }

        // Check if lost
        if (lives == 1) {
            printf("You lose! The secret phrase was: %s\n", secret);
            return (0);
        }

        // Ask for guess
        char guess;
        guess = read_letter();
        // Check if guess valid
        int same_letter = 0;
        if (letters[(int) guess] == (char) 125 || letters[(int) guess] == (char) 126) {
            same_letter = 1;
        }

        while (
            is_lowercase_letter(guess) != true || guess < 97 || guess > 122 || same_letter == 1) {
            same_letter = 0;
            guess = read_letter();
            if (letters[(int) guess] == (char) 125 || letters[(int) guess] == (char) 126) {
                same_letter = 1;
            }
        }

        // Check if guessed is correct
        if (string_contains_character(secret, guess) == true) {
            for (i = 0; i < 124; i++) {
                if (guess == (char) i) {
                    letters[i] = (char) 126;
                }
            }
        }

        else {
            for (i = 0; i < 124; i++) {
                if (guess == (char) i) {
                    letters[i] = (char) 125;
                    lives -= 1;
                }
            }
        }

        /*
		for (i = 0; i < 128; i++) {
                        printf("%c", letters[i]);
                }
		*/
    }
    return 0;
}

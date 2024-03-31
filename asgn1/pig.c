#include <stdio.h>
#include <stdlib.h>

int main(void) {

    /*
	* Input number of players using scanf()
	*/
    int num_players = 2;
    printf("Number of players (2 to 10)? ");
    int scanf_result = scanf("%d", &num_players);

    /*
	* If invalid number entered print error message
	* and inform the game will play with 2 players
	*/
    if (scanf_result < 1 || num_players < 2 || num_players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        num_players = 2;
    }

    /*
	* Input random seed using scanf()
	*/
    unsigned seed = 2023;
    printf("Random-number seed? ");
    scanf_result = scanf("%u", &seed);

    /*
	* If invalid seed then print error message and inform seed 2023 is being used
	*/
    if (scanf_result < 1) {
        fprintf(stderr, "Invalid seed. Using 2023 instead.\n");
        seed = 2023;
    }

    /*
	* Set random seed
	*/
    srandom(seed);

    /*
	* Array of player names
	*/
    const char *player_name[] = {
        "Margaret Hamilton",
        "Katherine Johnson",
        "Joy Buolamwini",
        "Grace Hopper",
        "Adele Goldberg",
        "Annie Easley",
        "Jeannette Wing",
        "Mary Kenneth Keller",
        "Megan Smith",
        "Radia Perlman",
    };

    /*
	* Set all players score = 0
	*/
    int score[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    /*
	* Start rolling
	*/
    int i = 0;
    int side = 0;
    int j = 0;
    while (score[0] < 100 && score[1] < 100 && score[2] < 100 && score[3] < 100 && score[4] < 100
           && score[4] < 100 && score[5] < 100 && score[6] < 100 && score[7] < 100 && score[8] < 100
           && score[9] < 100) {
        printf("%s\n", player_name[i]);
        j = i + 1;
        for (; i < j;) {
            side = random() % 7;
            if (side == 0 || side == 1) {
                printf(" rolls 0, has %d\n", score[i]);
                if (i == num_players - 1) {
                    i = 0;
                    j = 0;
                } else {
                    i++;
                }
            }

            else if (side == 2 || side == 3) {
                score[i] += 10;
                printf(" rolls 10, has %d\n", score[i]);
                if (score[i] >= 100) {
                    printf("%s won!\n", player_name[i]);
                    i++;
                }
            }

            else if (side == 4) {
                score[i] += 15;
                printf(" rolls 15, has %d\n", score[i]);
                if (score[i] >= 100) {
                    printf("%s won!\n", player_name[i]);
                    i++;
                }
            }

            else {
                score[i] += 5;
                printf(" rolls 5, has %d\n", score[i]);
                if (score[i] >= 100) {
                    printf("%s won!\n", player_name[i]);
                    i++;
                }
            }
        }
    }
    return 0;
}

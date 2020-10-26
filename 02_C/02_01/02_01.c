// one letter exactly twice
// one letter exactly three times


// ascii:
// a -> 97
// z -> 122

#include <stdio.h>
#include <stdlib.h>

#define TEST_RUN 0

#if(TEST_RUN == 1)
    #define INPUT_LENGTH 6
#else
    #define INPUT_LENGTH 26
#endif

#define INPUT_OFFSET 2
#define ASCII_LETTER_START 97
#define LETTER_IN_ALPHABET_COUNT 26


void count_letter(int * letters_count, const char letter)
{
    int asciiLetterNumber = (int)letter;

    letters_count[asciiLetterNumber - ASCII_LETTER_START]++;
}

void evaluate_letter_count(int * twice_count, int * three_times_count, const int * letters_count)
{
    for (int i = 0; i < LETTER_IN_ALPHABET_COUNT; i++)
    {
        if(letters_count[i] == 2)
        {
            *twice_count = 1;
        }
        else if (letters_count [i] == 3)
        {
            *three_times_count = 1;
        }
        else
        {
            // QAC
        }
    }
    
}

int main(void)
{
    FILE *fp;
    char line[INPUT_LENGTH+INPUT_OFFSET];

#if(TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\02_C\\02_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\02_C\\02_01\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    int total_twice = 0;
    int total_threeTimes = 0;

    while(fgets(line, sizeof(line), fp))
    {
        int twice_count = 0;
        int three_times_count = 0;

        char *single_letter;
        printf("%s", line);

        // 24 letters possible
        int letters_count[LETTER_IN_ALPHABET_COUNT] = {0};

        for(int i = 0; i < INPUT_LENGTH; i++)
        {
            count_letter(&letters_count, *(line+i));
        }

        evaluate_letter_count(&twice_count, &three_times_count, &letters_count);
        total_twice += twice_count;
        total_threeTimes += three_times_count;
    }

    int total = total_threeTimes * total_twice;

    printf("\nResult: %d", total);

    fclose(fp);
    exit(0);
}

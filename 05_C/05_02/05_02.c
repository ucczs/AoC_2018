#include <stdio.h>
#include <stdlib.h>


#define TEST_RUN 0
#define ASCII_DIFFERENCE_CAPITALIZATION 32
#define LENGTH_ALPHABET 26
#define ASCII_ALPHABET_START 65

#if(TEST_RUN == 1)
    #define NUMBER_OF_POLYMER 17
#else
    #define NUMBER_OF_POLYMER 50001
#endif

// difference capital/small letters in Ascii:
// A -> 65
// a -> 97
// minus 32
int removeReactionProducts(char *initial, char *reduced)
{
    int reducedPolymerCnt = 0;
    int changedFlag = 0;

    for (int i = 0; i < NUMBER_OF_POLYMER - 1; i++)
    {
        char currentLetter_c = initial[i];
        char nextLetter_c = initial[i+1];

        int currentLetter = (int)currentLetter_c;
        int nextLetter = (int)nextLetter_c;

        if( (currentLetter - ASCII_DIFFERENCE_CAPITALIZATION == nextLetter) ||
        (currentLetter + ASCII_DIFFERENCE_CAPITALIZATION == nextLetter) )
        {
            i++;
            changedFlag = 1;
        }
        else
        {
            reduced[reducedPolymerCnt] = currentLetter_c;
            reducedPolymerCnt++;
        }
    }

    return(changedFlag);
}

int countReducedPolymer(char *reduced)
{
    int polymerCnt = 0;

    for (int i = 0; i < NUMBER_OF_POLYMER; i++)
    {
        if( ((int)reduced[i] > 64) && ((int)reduced[i] < 123))
            polymerCnt++;
    }
    return(polymerCnt);
}

void preManipulatingPolymer(char *original, char *reduced, int skipLetter)
{
    int newCnt = 0;

    for (int i = 0; i < NUMBER_OF_POLYMER; i++)
    {
        char currentLetter_c = original[i];
        int currentLetter = (int)currentLetter_c;
        
        if( (currentLetter - ASCII_DIFFERENCE_CAPITALIZATION == skipLetter) ||
            (currentLetter + ASCII_DIFFERENCE_CAPITALIZATION == skipLetter) ||
            (currentLetter                                   == skipLetter) )
        {
            continue;
        }
        else
        {
            reduced[newCnt] = currentLetter_c;
            newCnt++;
        }
    }
}

int main(void)
{
    FILE *fp;

    char original[NUMBER_OF_POLYMER];
    char initial[NUMBER_OF_POLYMER];
    char reduced[NUMBER_OF_POLYMER];

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\05_C\\05_02\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\05_C\\05_02\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    while(fgets(original, sizeof(original), fp) != NULL)
    {

        for (int i = 0; i < LENGTH_ALPHABET; i++)
        {
            int changedFlag = 1;
            int removeLetterAscii = i + ASCII_ALPHABET_START;

            memset(&initial[0], 0, sizeof(initial) );
            memset(&reduced[0], 0, sizeof(reduced) );

            preManipulatingPolymer(&original[0], &initial[0], removeLetterAscii);

            while (changedFlag == 1)
            {
                changedFlag = removeReactionProducts(&initial[0], &reduced[0]);

                if(changedFlag == 1)
                {
                    memcpy(initial, reduced, NUMBER_OF_POLYMER * sizeof(char) );
                    memset(&reduced[0], 0, sizeof(reduced) );
                }
            }

            int countResult = countReducedPolymer(&reduced[0]);
            printf("Remaining Polymer: %d\n", countResult);
        }
    }
    return(0);
}
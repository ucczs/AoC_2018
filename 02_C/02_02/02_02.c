// one letter exactly twice
// one letter exactly three times


// ascii:
// a -> 97
// z -> 122

#include <stdio.h>
#include <stdlib.h>

#define TEST_RUN 0

#if(TEST_RUN == 1)
    #define INPUT_LENGTH 5
#else
    #define INPUT_LENGTH 26
#endif

#define INPUT_OFFSET 2
#define ASCII_LETTER_START 97
#define LETTER_IN_ALPHABET_COUNT 26

typedef struct {
  char **array;
  size_t used;
  size_t size;
} IDList;

void init_IDList(IDList *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(char*));
    a->used = 0;
    a->size = initialSize;
}

void insertIDToList(IDList *a, char *element)
{
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(char*));
     }
    a->array[a->used++] = element;
}

void free_list(IDList *a)
{
    for (int i = 0; i < a->used; i++)
    {
        free(a->array[i]);
    }
}

int compareTwoIDs(char *ID1, char *ID2)
{
    int unequal_cnt = 0;

    for (int letter_cnt = 0; letter_cnt < INPUT_LENGTH; letter_cnt++)
    {
        if( ID1[letter_cnt] != ID2[letter_cnt] )
        {
            unequal_cnt++;
        }

        if(unequal_cnt > 1)
        {
            break;
        }
    }

    return unequal_cnt;
}

void printID(char *ID)
{
    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        printf( "%c", *(ID + i) );
    }
    printf("\n");
}

void findMatchingIDs(IDList *a)
{
    for (int ID_cnt1 = 0; ID_cnt1 < a->used; ID_cnt1++)
    {
        char *currentCandidate = *(a->array + ID_cnt1);

        for (int ID_cnt2 = ID_cnt1 + 1; ID_cnt2 < a->used; ID_cnt2++)
        {
            char *loopingCandidate = *(a->array + ID_cnt2);
            int unequal_cnt = compareTwoIDs(currentCandidate, loopingCandidate);

            if(unequal_cnt < 2)
            {
                printf("Found IDs: \n");
                printID(currentCandidate);
                printID(loopingCandidate);
                printf("\n");
            }
        }
    }
}

int main(void)
{
    FILE *fp;
    char line[INPUT_LENGTH+INPUT_OFFSET];

    IDList IDCandidates;
    init_IDList(&IDCandidates, 200);

#if(TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\02_C\\02_02\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\02_C\\02_02\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    while(fgets(line, sizeof(line), fp))
    {
        char *candidate = (char*)calloc(INPUT_LENGTH, sizeof(char));
        memcpy(candidate, line, INPUT_LENGTH * sizeof(char) );
        insertIDToList(&IDCandidates, candidate);
    }

    findMatchingIDs(&IDCandidates);

    free_list(&IDCandidates);
    fclose(fp);
    exit(0);
}
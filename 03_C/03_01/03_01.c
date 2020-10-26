// how many inches of fabric are within two or more claims?

#include <stdio.h>
#include <stdlib.h>

#define TEST_RUN 0
#define MAX_INPUT_LENGTH 24

// width -> expansion in x direction
// length -> exapansion in y direction
typedef struct{
    int ID;
    int xPos;
    int yPos;
    int width;
    int length;
}claim;

typedef struct{
    claim *array;
    int size;
    int used;
}claimList;

typedef struct{
    int *mapVal;
    int maxWidth;
    int maxLength;
}Map;

void init_ClaimList(claimList *a, size_t initialSize)
{
    a->array = (claim *) malloc(initialSize * sizeof(claim));
    a->used = 0;
    a->size = initialSize;
}

void insertClaimToList(claimList *a, claim element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(claim));
    }
    a->array[a->used++] = element;
}

void getID(char *line, claim *newClaim)
{
    int IDStarted = 0;
    int IDEnded = 0;
    int char_cnt = 0;
    int found_ID = 0;

    while(IDEnded == 0)
    {
        if(*(line + char_cnt) == '#')
        {
            IDStarted = 1;
        }
        else if(*(line + char_cnt) == ' ')
        {
            IDEnded = 1;
        }
        else
        {
            found_ID *= 10;
            found_ID += ( *(line + char_cnt) - '0');
        }

        char_cnt++;
    }

    newClaim->ID = found_ID;
}

void getPos(char *line, claim *newClaim)
{
    int XPosFlag = 0;
    int YPosFlag = 0;
    int stopFlag = 0;

    int foundXPos = 0;
    int foundYPos = 0;

    int char_cnt = 0;

    while(stopFlag == 0)
    {
        if(*(line + char_cnt) != '#')
        {
            if( (*(line + char_cnt) == ' ') && (*(line + char_cnt - 1) == '@') )
            {
                XPosFlag = 1;
            }
            else if( (XPosFlag == 1) && ( *(line + char_cnt) != ',' ) )
            {
                // XPos
                foundXPos *= 10;
                foundXPos += ( *(line + char_cnt) - '0');
            }
            else if(*(line + char_cnt) == ',')
            {
                XPosFlag = 0;
                YPosFlag = 1;
            }
            else if( (YPosFlag == 1) && (*(line + char_cnt) != ':' ) )
            {
                // XPos
                foundYPos *= 10;
                foundYPos += ( *(line + char_cnt) - '0');
            }
            else if( *(line + char_cnt) == ':' )
            {
                YPosFlag = 0;
                stopFlag = 1;
            }
            else
            {
                // QAC
            }
        }
        char_cnt++;
    }

    newClaim->xPos = foundXPos;
    newClaim->yPos = foundYPos;
}

void getDimensions(char *line, claim *newClaim)
{
    int WidthFlag = 0;
    int LengthFlag = 0;
    int stopFlag = 0;

    int foundWidth = 0;
    int foundLength = 0;

    int char_cnt = 0;

    while(stopFlag == 0)
    {
        if( *(line + char_cnt) != '#' )
        {
            if( (*(line + char_cnt) == ' ') && (*(line + char_cnt - 1) == ':') )
            {
                WidthFlag = 1;
            }
            else if( (WidthFlag == 1) && ( *(line + char_cnt) != 'x' ) )
            {
                // Width
                foundWidth *= 10;
                foundWidth += ( *(line + char_cnt) - '0');
            }
            else if(*(line + char_cnt) == 'x')
            {
                WidthFlag = 0;
                LengthFlag = 1;
            }
            else if( (LengthFlag == 1) && (*(line + char_cnt) != '\n' ) )
            {
                // Width
                foundLength *= 10;
                foundLength += ( *(line + char_cnt) - '0');
            }
            else if( *(line + char_cnt) == '\n' )
            {
                LengthFlag = 0;
                stopFlag = 1;
            }
            else
            {
                // QAC
            }
        }
        char_cnt++;
    }

    newClaim->width = foundWidth;
    newClaim->length = foundLength;
}

// example: #1 @ 1,3: 4x4
void processInputLine(char *line, claimList *cList)
{
    if( *line == '#' )
    {
        claim newClaim;

        getID(line, &newClaim);
        getPos(line, &newClaim);
        getDimensions(line, &newClaim);

        insertClaimToList(cList, newClaim);
    }
}

void calculateMapSize(Map *fabricMap, claimList *cList)
{
    int max_length = 0;
    int max_width = 0;

    for (int i = 0; i < cList->used; i++)
    {
        int claim_width = cList->array[i].xPos + cList->array[i].width;
        int claim_length = cList->array[i].yPos + cList->array[i].length;

        if(claim_width > max_width)
        {
            max_width = claim_width;
        }

        if(claim_length > max_length)
        {
            max_length = claim_length;
        }

    }
    fabricMap->maxLength = max_length + 1;
    fabricMap->maxWidth = max_width + 1;
}

void createMap(Map *fabricMap)
{
    int *map = (int*) calloc( fabricMap->maxLength * fabricMap->maxWidth, sizeof(int) );
    fabricMap->mapVal = map;
}

void fillMapWithClaims(Map *fabricMap, claimList *cList)
{
    for (int claim_cnt = 0; claim_cnt < cList->used; claim_cnt++)
    {
        claim currentClaim = cList->array[claim_cnt];
        for (int i = 0; i < currentClaim.length; i++)
        {
            for (int j = 0; j < currentClaim.width; j++)
            {
                int xPosFabric = currentClaim.xPos;
                int yPosFabric = currentClaim.yPos;
                int index = fabricMap->maxWidth * (i + yPosFabric) + xPosFabric + j;

                fabricMap->mapVal[index]++;
            }
        }
    }
}

int countOverlapping(Map *fabricMap)
{
    int overlap_cnt = 0;
    for (int i = 0; i < fabricMap->maxLength * fabricMap->maxWidth; i++)
    {
        if(fabricMap->mapVal[i] > 1)
        {
            overlap_cnt++;
        }
    }
    return overlap_cnt;
}

int main(void)
{
    FILE *fp;
    char line[MAX_INPUT_LENGTH];
    int overlapping_area = 0;

    Map fabricMap;

    claimList cList;
    init_ClaimList(&cList, 1);

#if(TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\03_C\\03_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\03_C\\03_01\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    while(fgets(line, MAX_INPUT_LENGTH, fp) != NULL)
    {
        processInputLine(&line[0], &cList);
    }

    calculateMapSize(&fabricMap, &cList);
    createMap(&fabricMap);
    fillMapWithClaims(&fabricMap, &cList);

    overlapping_area = countOverlapping(&fabricMap);
    printf("Overlapping area: %d", overlapping_area);

    free(fabricMap.mapVal);
    free(cList.array);

    fclose(fp);
    return(0);
}
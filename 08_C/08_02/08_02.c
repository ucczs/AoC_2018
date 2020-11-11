#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TEST_RUN 0

#if( TEST_RUN == 1 )
    #define INPUT_LENGTH 36
#else
    #define INPUT_LENGTH 32975
#endif

typedef struct TreeNode TreeNode;

struct TreeNode{
    int Count_ChildNodes;
    int Count_MetaData;

    TreeNode* children;
    int* MetaData;
    int sum_MetaData;
};

int getNextValidNumber(char *line, int* startIndex)
{
    bool numberAcquisitionDone = false;
    bool numberFound = false;
    int foundValue = 0;

    while(numberAcquisitionDone == false)
    {
        if( (*startIndex >= INPUT_LENGTH-1) || ( (line[*startIndex ] == ' ') && (numberFound == true) ))
        {
            numberAcquisitionDone = true;
        }
        else if( line[*startIndex] == 'x' || (line[*startIndex] == ' ') )
        {

        }
        else
        {
            foundValue *= 10;
            foundValue += ( (int)line[*startIndex] - 48 );
            numberFound = true;
            line[*startIndex] = 'x';
        }

        (*startIndex)++;
    }

    return foundValue;
}

TreeNode* generateNode(char *line, TreeNode* currentNode, int *lineIdx)
{
    // do it recursively

    bool nodeComplete = false;

    currentNode->Count_ChildNodes = getNextValidNumber(line, lineIdx);
    currentNode->Count_MetaData = getNextValidNumber(line, lineIdx);

    TreeNode* newChildren = (TreeNode*)calloc(currentNode->Count_ChildNodes, sizeof(TreeNode));
    currentNode->children = newChildren;
    for (int i = 0; i < currentNode->Count_ChildNodes; i++)
    {
        TreeNode* childNode = generateNode(line, &newChildren[i], lineIdx);
    }

    int* newMetaData = (int*)calloc(currentNode->Count_MetaData, sizeof(int));
    currentNode->MetaData = newMetaData;
    currentNode->sum_MetaData = 0;
    for (int i = 0; i < currentNode->Count_MetaData; i++)
    {
        newMetaData[i] = getNextValidNumber(line, lineIdx);
        currentNode->sum_MetaData += newMetaData[i];
    }

    if( currentNode->Count_ChildNodes != 0 )
    {
        currentNode->sum_MetaData = 0;

        for (int j = 0; j < currentNode->Count_MetaData; j++)
        {
            int currentMetaData = currentNode->MetaData[j];
            if((currentMetaData != 0) && (currentMetaData <= currentNode->Count_ChildNodes) )
            {
                currentNode->sum_MetaData += currentNode->children[currentMetaData-1].sum_MetaData;
            }
        }
    }

    return(newChildren);
}

int main(void)
{
    FILE *fp;
    char line[INPUT_LENGTH];

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\08_C\\08_02\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\08_C\\08_02\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    fgets(line, sizeof(line), fp);

    int lineIdx = 0;

    TreeNode* rootNode = (TreeNode*)calloc(1, sizeof(TreeNode));
    generateNode(&line[0], rootNode, &lineIdx);

    printf("Meta Data Sum of root node: %d\n", rootNode->sum_MetaData);

    return(0);
}
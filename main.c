#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dataBase.h"
#include "constantVar.h"
#include "Utility.h"
#include "preAssembly.h"
#include "stage1.h"
#include "stage2.h"




int main(int argc, char *argv[] )
{
    FILE *object;
    line name, line1;
    Pips *IC = (Pips*)malloc(sizeof(Pips) * STORAGE_FOR_EACH);
    Pips *DC = (Pips*)malloc(sizeof(Pips) * STORAGE_FOR_EACH);
    LabelNode *labels = creatLabelTableList();
    Nextern * Nexternal = creatExternIntryList();
    char ** icOct;
    char ** dcOct;
    int * continueToNext;
    int nameIndex, index;


    nameIndex = 1;
    while(argv[nameIndex] != NULL)
    {
        strcpy(name, /*"Test1"*/argv[nameIndex]);

        preAssembly(name);
        continueToNext = stage1(name, &IC, &DC, &Nexternal, &labels);

        if (!(continueToNext[0] == FALSE_VALUE && continueToNext[1] == FALSE_VALUE))
            stage2(name, &IC, &labels, &Nexternal);
        else {
            printf("Error|5|: There is a problem in main method with stage1");
            exit(1);
        }

        makeFile(&Nexternal, 'i', "Test1");   // i stands for entrys
        makeFile(&Nexternal, 'e', "Test1");   // e stand for extern

        icOct = binarToOct(&IC, continueToNext[C_IC]);
        dcOct = binarToOct(&DC, continueToNext[C_DC]);

        strcpy(line1, name);
        object = fopen(strcat(line1, ".ob"), "w");
        fprintf(object, "\t\t%d  %d\n", continueToNext[C_IC], continueToNext[C_DC]);

        for (index = 0; index < continueToNext[C_IC]; index++) {
            fprintf(object, "\t%d  %s\n", (index + START_OF_IC), icOct[index]);
        }

        for (index = 0; index < continueToNext[C_DC]; index++) {
            fprintf(object, "\t%d  %s\n", (index + START_OF_IC + continueToNext[C_IC] ), dcOct[index]);
        }

        strcpy(line1, name);
        fclose(object);
        remove(strcat(line1, ".am"));

        freeLabelTableList((&labels));
        freeOct(&icOct);
        freeOct(&dcOct);
        free(IC);
        free(DC);
        freeNexternList(&Nexternal);
        free(continueToNext);
        labels = NULL;
        IC = NULL;
        DC = NULL;
        nameIndex++;
    }

}
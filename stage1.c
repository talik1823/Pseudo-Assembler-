//
// Created by Tal on 24/07/2024.
//

#include "Utility.h"
#include "stage1.h"
#include "dataBase.h"
#include "constantVar.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



int *stage1(char *file ,Pips ** IC, Pips **DC, Nextern **NexternalTable , LabelNode **labelList)
{

    FILE *nFile;
    LabelNode *head = *labelList;    // Lable List
    Nextern *externTable = *NexternalTable;  // table of the external labels
    char **buffer;  // holds all the words the line contain
    int ic = 0, dc = 0;
    int dataType, index, space;
    int * conutersFinalValue ;
    char labelFlag = FALSE_VALUE;
    char thereIsError = FALSE_VALUE;
    char Nexternal;
    Pips Pcode;
    line tamp;
    line fFile;


    strcpy(fFile,file);
    nFile = fopen(strcat(fFile, ".am"), "r");

    if(nFile == NULL)
    {
        printf("Error1: the file %s is not exist\n",fFile);
        thereIsError = TRUE_VALUE;
    }

    while( fgets(tamp, SIZE_OF_LINE, nFile) != NULL) {

        if(tamp[strlen(tamp)-1] == '\n')
            tamp[strlen(tamp)-1] = '\0';

        buffer = wordOfLine(tamp);

        if (/* 1 */buffer[0] == NULL || /* 2 */buffer[0][0] == ';')   // 1. if the line is empty or have only white spaces
            continue;                                 // 2. or if the line is note in the code.

        if (isLabel(buffer[0]))
            labelFlag = TRUE_VALUE;


        dataType = isData(&buffer); // there is a data operation?
        if (dataType)   //there is a data operation in the line
        {

            if (labelFlag) {  // there is a label and also data operation
                                          /*   enter the label to the labels table with data sing and the DC address   */
                thereIsError = nameExist(&head,buffer[0])? TRUE_VALUE : FALSE_VALUE;
                setLabel(&head, buffer[0], dc + VAR_STARTING_POINT, TRUE_VALUE, TRUE_VALUE, spaceForWords(tamp));

            }

            /*  here you should enter all the data variable into the DC array */
            switch (dataType) {
                case DATA_WITHOUT_LABEL:
                    index = 1;
                    while (buffer[index] != NULL && dc != ERROR_VALUE)
                        dc = enterData(DC, (int)strtol(buffer[index++], NULL,10) ,  dc);


                    break;

                case DATA_WITH_LABEL:
                    index = 2;

                    while (buffer[index] != NULL && dc != ERROR_VALUE)
                        dc = enterData(DC, (int)strtol(buffer[index++], NULL,10),  dc);
                    break;

                case STRING_WITHOUT_LABEL:
                    index = 1;
                    while (buffer[index] != NULL && dc != ERROR_VALUE)
                        dc = enterString(DC, buffer[index++],   dc);
                    break;

                case STRING_WITH_LABEL:
                    index = 2;
                    while (buffer[index] != NULL && dc != ERROR_VALUE)
                        dc = enterString(DC, buffer[index++], dc);
                    break;

                default:
                    break;
            }

            if (dc == ERROR_VALUE) {
                printf("There is a problem with insert Data to the DC array\n");
                thereIsError = TRUE_VALUE;
            }

            freeWords(&buffer);
            labelFlag = FALSE_VALUE;
            continue;
        }


        if ( (Nexternal = isNextern(&buffer)) )
        {
            if(Nexternal == (ENTRY_LINE + 1))
            {
                index = 2;
                while(buffer[index] != NULL)
                {
                    setName(&externTable, buffer[index], FALSE_VALUE);   // set name of the extern list to create the extern file in the end
                    index++;
                }
            }

            if(Nexternal == ENTRY_LINE)
            {
                index = 1;
                while( buffer[index] != NULL)
                {
                    setName(&externTable, buffer[index], FALSE_VALUE);   // set name of the extern list to create the extern file in the end
                    index++;
                }
            }

            if(Nexternal == (EXTERN_LINE + 1))
            {
                index = 2;
                while(buffer[index] != NULL)
                {
                    setName(&externTable, buffer[index], TRUE_VALUE);   // set name of the extern list to create the extern file in the end
                    setLabel(&head, buffer[index], EXTERNAL_ADDRESS,FALSE_VALUE, FALSE_VALUE, ERROR_VALUE );
                    index++;
                }
            }

            if(Nexternal == EXTERN_LINE)
            {
                index = 1;
                while( buffer[index] != NULL)
                {
                    setName(&externTable, buffer[index], TRUE_VALUE);   // set name of the extern list to create the extern file in the end
                    setLabel(&head, buffer[index], EXTERNAL_ADDRESS, FALSE_VALUE, FALSE_VALUE,ERROR_VALUE);
                    index++;
                }
            }

            freeWords(&buffer);
            labelFlag = FALSE_VALUE;
            continue;
        }

        if( labelFlag )
        {
            if(nameExist(&head, buffer[0])) {
                printf("ERROR|2|:   the label \"%s\" is already exist\n ", buffer[0]);
                thereIsError = TRUE_VALUE;
            }
            else
                setLabel(&head, buffer[0], ic + VAR_STARTING_POINT, FALSE_VALUE, TRUE_VALUE, spaceForWords(tamp));
        }

        /*  1. valuate the space for the directive line |if ERROR: there isn't valid line |if exist number: save in space;
         *  2. assemble the first line only;
         *  3.
         *  */
        if( (space = (spaceForWords(tamp)) ) != ERROR_VALUE )   // this return ERROR_VALUE only in case the line is not valid
        {
            Pcode = createPips(tamp, labelFlag , space);
            (*IC)[START_OF_IC + ic] = Pcode;
            ic = ic + space;
        }
        else
        {
            printf("the line \"%s\" is not valid\n",tamp);
            thereIsError = TRUE_VALUE;
            freeWords(&buffer);
            labelFlag = FALSE_VALUE;
            continue;
        }

        labelFlag = FALSE_VALUE;
        freeWords(&buffer);
    }

    conutersFinalValue = (int*)malloc(sizeof(int) * COUNTERS_AMOUNT);
    if(thereIsError)    // in case of error we will stop the program
    {
     conutersFinalValue[0] = FALSE_VALUE;
     conutersFinalValue[1] = FALSE_VALUE;
     return conutersFinalValue;
    }


    else{
        conutersFinalValue[0] = ic;
        conutersFinalValue[1] = dc;
        flipToCode( &head, ic);
        return conutersFinalValue;
    }
    /*  updating all the labels in the table that are data to code by changing the address of team to address = Ic Ic = Ic+ lable.DataLength*/
}


/*
 *
 * those function in the below are the functions that help the main function in this file stage1 who doing the process of the first
 * pass on the file
 *
 * */

int isLabel(char* line)
{
    int length = (int)strlen(line);
    char prifx = line[length-1];

    if(prifx == ':')
        return TRUE_VALUE;
    else
        return FALSE_VALUE;
}


int enterData(Pips ** DC, int variable, int dcCounter)
{
    Pips *ptr = *DC;
    Pips binaryValue;
    int reminder;
    int counter;
    int tamp = abs(variable);

    binaryValue.value = 0b0;

    if(tamp > VARIABLE_VALUE_LIMIT)
    {
        printf("the value you want to enter is to big for the system which can handle numbers not more then %d\n", (int)(pow(2,14)-1));
        return ERROR_VALUE;
    }

    counter = 0;
    while(tamp != 0)
    {
        reminder = tamp % 2;
        reminder = reminder<<(counter++);
        binaryValue.value = binaryValue.value | reminder;
        tamp = tamp / 2;
    }

    if(variable < 0)    // using the Two's complement method
        binaryValue.value = ~binaryValue.value  + 1;
    if(dcCounter < STORAGE_LIMIT)
        ptr[(dcCounter++) + VAR_STARTING_POINT].value = binaryValue.value;
    else
    {
        printf("there is overflow in the memory on the data array please increase your data memory\n");
        return ERROR_VALUE;
    }
    return dcCounter;
}

int enterString(Pips **DC, char * string, int dcCounter)
{
    line tamp;
    int i;

    if(string[0] == '"' && string[ strlen(string) -1 ] == '"')
    {
        strncpy(tamp, string + 1, strlen(string) - 2);
        tamp[strlen(string) -2] = '\0';
    }

    for(i = 0; i < strlen(tamp)+1 && dcCounter != ERROR_VALUE; i++)
        dcCounter = enterData(DC, tamp[i], dcCounter) ;

    if( dcCounter == ERROR_VALUE )
        return ERROR_VALUE;

    return dcCounter;
}

Pips createPips(char *word, char label, int space)
{
    HashTable *table = createHashTable();
    char **buffer = wordOfLine(word);
    Pips tamp = { 0b000000000000000 };
    setup_Table(&table);

    tamp.value = (search(&table, buffer[label]))->value | tamp.value;
    tamp.value = tamp.value | a.value;  // first word always from A filed
    space--;
    if( space == 2 || bothRegister( &buffer ) )
    {
        switch( whatAddresing(buffer[label + 1]) )
        {
            case FIRST_ADDRESING_METHOD:
                tamp.value = tamp.value | s_zero.value;
                break;
            case SECOND_ADDRESING_METHOD:
                tamp.value = tamp.value | s_one.value;
                break;
            case THIRD_ADDRESING_METHOD:
                tamp.value = tamp.value | s_two.value;
                break;
            case FORTH_ADDRESSING_METHOD:
                tamp.value = tamp.value | s_three.value;
                break;
            }
        switch( whatAddresing(buffer[label + 2]) )
        {
            case FIRST_ADDRESING_METHOD:
                tamp.value = tamp.value | d_zero.value;
                break;
            case SECOND_ADDRESING_METHOD:
                tamp.value = tamp.value | d_one.value;
                break;
            case THIRD_ADDRESING_METHOD:
                tamp.value = tamp.value | d_two.value;
                break;
            case FORTH_ADDRESSING_METHOD:
                tamp.value = tamp.value | d_three.value;
                break;
        }
    }
    else if( space == 1 )
    {
        switch( whatAddresing(buffer[label + 1]) )
        {
            case FIRST_ADDRESING_METHOD:
                tamp.value = tamp.value | d_zero.value;
                break;
            case SECOND_ADDRESING_METHOD:
                tamp.value = tamp.value | d_one.value;
                break;
            case THIRD_ADDRESING_METHOD:
                tamp.value = tamp.value | d_two.value;
                break;
            case FORTH_ADDRESSING_METHOD:
                tamp.value = tamp.value | d_three.value;
                break;
        }
    }
    return tamp;
}



void flipToCode(LabelNode **labelList, int ic)
{
    LabelNode *tamp = *labelList;
    int dcTamp = 0;

    while( tamp != NULL)
    {
        if(tamp->Data)  //  if the label is in the data section
        {
          ic = ic + tamp->address - dcTamp;
          dcTamp = tamp->address + tamp->DataLength;
          tamp->address = ic;
          tamp->Data = FALSE_VALUE;
          ic = ic + tamp->DataLength;
        }
        tamp = tamp->next;
    }/*
 *      tamp->next->address = tamp->DataLength + tamp->address
 *
 * */
}

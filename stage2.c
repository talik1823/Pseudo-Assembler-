//
// Created by Tal on 15/10/2024.
//
#include "Utility.h"
#include "stage1.h"
#include "dataBase.h"
#include "constantVar.h"
#include "stage2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int stage2(char *file, Pips **IC, LabelNode ** labelList, Nextern **NexternList)
{
    FILE *nFile;
    Pips *Ic = *IC;
    LabelNode *head = *labelList;
    Nextern  * nexternTabel = *NexternList;
    line fFile, tamp;
    int space, index,thereIsLabel = FALSE_VALUE,ic = 0;
    char thereIsError = FALSE_VALUE;
    char **buffer;
    int length;

    strcpy(fFile, file);
    nFile = fopen(strcat(fFile,".am"), "r");

    if( nFile == NULL)
    {
        printf("Error|3|:there is a problem with opening the file.\n");
        thereIsError = TRUE_VALUE;
    }

    while(fgets(tamp,SIZE_OF_LINE,nFile) != NULL )
    {
        buffer = wordOfLine(tamp);
        space = spaceForWords(tamp);
        thereIsLabel = isLabel(buffer[0]);

        if( tamp[length = (int)(strlen(tamp) - 1)] ==  '\n')
            tamp[length] = '\0';

        if( thereIsName(&nexternTabel, buffer[0]) )
            addAddress(&nexternTabel, buffer[0], START_OF_IC + ic);

        if (/* 1 */buffer[0] == NULL || /* 2 */buffer[0][0] == ';')   // 1. if the line is empty or have only white spaces
            continue;                                 // 2. or if the line is note in the code.

        /*  if the line is .extern/.data/.string we will continue in the reading   */
        if(isData(&buffer) || isNextern(&buffer) == EXTERN_LINE || isNextern(&buffer) == (EXTERN_LINE +1))
            continue;

        if( isNextern(&buffer) == ENTRY_LINE )
        {
            index = 1;
            while (buffer[index] != NULL)
                makeEntry(&head, buffer[index++]);
            continue;
        }

        if( thereIsLabel )
            thereIsError = thereIsError || (nameExist(&head, buffer[0]))? FALSE_VALUE : TRUE_VALUE;

        ic++;   // skipping on the first word
        index = 1;  // there is no Label
        while (buffer[thereIsLabel + index] != NULL)
        {


            switch( whatAddresing(buffer[thereIsLabel +index]))
            {
                case FIRST_ADDRESING_METHOD:

                    enterFirstAdressing(IC, ic, buffer[thereIsLabel + index]);
                    ic++;
                    index++;
                    break;

                case SECOND_ADDRESING_METHOD:

                    if(thereIsName(&nexternTabel, buffer[thereIsLabel + index]))
                        addAddress(&nexternTabel, buffer[thereIsLabel + index], START_OF_IC + ic);

                    enterSecondAdressing(IC, &head, ic, buffer[thereIsLabel + index]);
                    ic++;
                    index++;
                    break;

                case THIRD_ADDRESING_METHOD:

                    if( bothRegister( &buffer ) )
                    {
                        if(  index == 1 ) // this mean we are in the first register out of two and we should not increase the ic indicator.
                            enterThirdAdressing(IC, ic, buffer[thereIsLabel + index], FALSE_VALUE, SOURCE_PLACE_REGISTER);
                        if( index == 2 ) // this mean we are in the second register out of two, and therefore we should increase the  ic indicator.
                        {
                            enterThirdAdressing(IC, ic, buffer[thereIsLabel + index], FALSE_VALUE, DESTNTION_PLACE_REGISTER);
                            ic++;
                        }
                        index++;
                        break;
                    }

                    if( space == 2 )  //if there is a only 2 words, so it mean there is only 1 register operand that should be
                                // in the destination section(3-5)
                        enterThirdAdressing(IC, ic, buffer[thereIsLabel + index], TOW_WORDS_DEST , FALSE_VALUE);

                    if( space == 3 )  // put in the state 1 or 2 depend on the index you are in right now
                    {
                        if(index == 1)
                            enterThirdAdressing(IC, ic, buffer[thereIsLabel + index], THREE_WORDS_SOURCE , FALSE_VALUE);

                        if(index == 2)
                            enterThirdAdressing(IC,ic,buffer[thereIsLabel + index], THREE_WORDS_DEST, FALSE_VALUE);
                    }
                    ic++;
                    index++;
                    break;

                case FORTH_ADDRESSING_METHOD:

                    if( bothRegister( &buffer ) )
                    {
                        if( index == 1 )
                            enterFourthAdressing(IC, ic, buffer[thereIsLabel + index], FALSE_VALUE, SOURCE_PLACE_REGISTER);
                        if( index == 2 )
                        {
                            enterFourthAdressing(IC, ic, buffer[thereIsLabel +index], FALSE_VALUE, DESTNTION_PLACE_REGISTER);
                            ic++;
                        }
                        index++;
                        break;
                    }

                    if( space == 2 )
                        enterFourthAdressing(IC, ic, buffer[thereIsLabel + index], TOW_WORDS_DEST, FALSE_VALUE);

                    if( space == 3 )
                    {
                        if( index == 1)
                            enterFourthAdressing(IC, ic, buffer[thereIsLabel + index], THREE_WORDS_SOURCE, FALSE_VALUE);

                        if(index == 2)
                            enterFourthAdressing(IC, ic, buffer[thereIsLabel + index], THREE_WORDS_DEST, FALSE_VALUE);
                    }
                    index++;
                    ic++;
                    break;

                default:
                    printf("The code reaches the default section in \"switch |132|\"\n");
                    index++;
                    break;
            }
        }
        freeWords( &buffer );
    }
    if(thereIsError)
        return FALSE_VALUE;
    else
        return TRUE_VALUE;
}

void enterFirstAdressing(Pips **IC, int ic, char* operand)
{
    Pips *Ic = *IC;
    Pips tamp = {0b000};
    char * numberString = (operand+1);  // the first addressing method comes with '#' in the start of the line, therefore we will want to forward one char

    tamp.value = (int)strtol(numberString, NULL,10);
    tamp.value = tamp.value<<3;
    tamp.value = tamp.value | a.value;
    Ic[START_OF_IC + ic].value = tamp.value;
}

void enterSecondAdressing(Pips **IC, LabelNode **List, int ic, char* name)
{
    Pips *Ic = *IC;
    Pips tamp = {0b000000};
    LabelNode *head = *List;
    int address;

    if( ( address = getAddress(&head, name) ) == EXTERNAL_ADDRESS)
    {
        tamp.value = tamp.value | e.value;
        Ic[START_OF_IC + ic].value = tamp.value;
        return;
    }

    tamp.value = address;
    tamp.value = tamp.value<<3;
    tamp.value = tamp.value | r.value;
    Ic[START_OF_IC + ic].value = tamp.value;
}

void enterThirdAdressing(Pips **IC, int ic, char* operand, char state, char ifBothRegister)
{
    Pips *Ic = *IC;
    Pips tamp = {0b00000};
    char * word;
    int registerNumber = (int) strtol(operand + 2,NULL,10);

    switch (ifBothRegister)
    {
        case SOURCE_PLACE_REGISTER:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 6;
            tamp.value = tamp.value | a.value;
            Ic[START_OF_IC + ic].value = tamp.value;
            return;

        case DESTNTION_PLACE_REGISTER:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 3;
            Ic[START_OF_IC + ic].value = Ic[ic + START_OF_IC].value | tamp.value;
            return;

        case FALSE_VALUE:
            break;

        default:
            printf("The code reaches the default section in \"switch |171|\"\n");
            break;

    }

   switch( state )
   {
       case TOW_WORDS_DEST:
           tamp.value = registerNumber;
           tamp.value = tamp.value<<3;  // the number of the register need to be in the destination part(3-5) when you have only one operand
           tamp.value = tamp.value | a.value;
           Ic[START_OF_IC + ic].value = tamp.value;
           break;
       case THREE_WORDS_SOURCE:
           tamp.value = registerNumber;
           tamp.value = tamp.value<<6;  // the number of the register need to be in the source part(6-8) when it's the first operand from two
           tamp.value = tamp.value | a.value;
           Ic[START_OF_IC + ic].value = tamp.value;
           break;
       case THREE_WORDS_DEST:
           tamp.value = registerNumber;
           tamp.value = tamp.value<<3;  // the number of the register need to be in the destination part(3-5) when it's the second operand from two
           tamp.value = tamp.value | a.value;
           Ic[START_OF_IC + ic].value = tamp.value;
           break;
       default:
           printf("there is a problem with cases in \"enterThirdAdressing\" function\n");
   }
}

void enterFourthAdressing(Pips **IC, int ic, char* operand, char state, char ifBothRegister)
{
    Pips *Ic = *IC;
    Pips tamp = {0b00000};
    int registerNumber = (int) strtol(operand + 1, NULL, 10);

    switch ( ifBothRegister )
    {
        case SOURCE_PLACE_REGISTER:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 6;
            tamp.value = tamp.value | a.value;
            Ic[START_OF_IC + ic].value = tamp.value;
            return;

        case DESTNTION_PLACE_REGISTER:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 3;
            Ic[START_OF_IC + ic].value = Ic[ic + START_OF_IC].value | tamp.value;
            return;

        case FALSE_VALUE:
            break;

        default:
            printf("The code reaches the default section in \"switch 202\"\n");
            break;
    }

    switch( state )
    {
        case TOW_WORDS_DEST:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 3;
            tamp.value = tamp.value | a.value;
            Ic[START_OF_IC + ic].value = tamp.value;
            break;

        case THREE_WORDS_SOURCE:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 6;
            tamp.value = tamp.value | a.value;
            Ic[START_OF_IC + ic].value = tamp.value;
            break;

        case THREE_WORDS_DEST:
            tamp.value = registerNumber;
            tamp.value = tamp.value << 3;
            tamp.value = tamp.value | a.value;
            Ic[START_OF_IC + ic].value = tamp.value;
            break;
        default:
            printf("The code reaches the default section in \"switch 253\"\n");
            break;
    }
}

//
// Created by Tal on 29/08/2024.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Utility.h"
#include "dataBase.h"
#include "stage1.h"

#define GROUP_ONE 1
#define GROUP_TWO 2
#define GROUP_THREE 3
#define GROUP_FOUR 4
#define GROUP_FIVE 5
#define GROUP_SIX 6
#define GROUP_SEVAN 7
#define BOTH_REGISTER 2
#define REGISTER_NUM 8

char** wordOfLine( char* line)
{
    char ** str = (char**) malloc(sizeof(char*) * SIZE_OF_LINE);
    int index = 0, indexWord = 0 , indexChar = 0;
    char * tampLine = (char*)malloc(SIZE_OF_LINE);

    strcpy(tampLine, line);
    while( tampLine[index] != '\0' && tampLine[index] != EOF && tampLine[index] != '\n' )
    {
        while( tampLine[index] == ' ' || tampLine[index] == ',')
            index++;

        if(tampLine[index] == '\0' || tampLine[index] == EOF)
            break;

        str[indexWord] = (char* ) malloc(SIZE_OF_LINE);
        indexChar = 0;

        while( (tampLine[index] != ' ') && (tampLine[index] != '\0') && (tampLine[index] != EOF) && (tampLine[index] != ',') && (tampLine[index] != '\n'))
            str[indexWord][indexChar++] = tampLine[index++];

        str[indexWord][indexChar] = '\0';
        indexWord++;
    }

    str[indexWord] = NULL;
    free(tampLine);
    return str;
}

void freeWords(char *** buffer)
{
    char ** buff = *buffer;
    int index;
    for(index = 0; buff[index] != NULL; index++)
        free( buff[index] );
    free(buff);
    buffer = NULL;
}

int isData(char *** buff)
{
    char ** buffer = *buff;
    int  boolean;

    boolean = (!strcmp(buffer[0], ".data"));
    if(boolean)
        return DATA_WITHOUT_LABEL;

    boolean = (!strcmp(buffer[0], ".string"));
    if( boolean )
        return STRING_WITHOUT_LABEL;

    boolean = (!strcmp(buffer[1], ".data"));
    if( boolean )
        return DATA_WITH_LABEL;

    boolean = ( !strcmp(buffer[1], ".string"));
    if( boolean )
        return STRING_WITH_LABEL;

    return FALSE_VALUE;
}

char isNextern(char*** buffer)   /*  is the line contain extern or entry command   */
{                               /*  return 2: if extern is without label.   return 4: if entry is without label */
    char ** buf = *buffer;      /*  return 2+1 or 4+1 if the line also contain label in addition to extern or entry*/
    char external = FALSE_VALUE;
    char entry = FALSE_VALUE;
    char label = FALSE_VALUE;

    external = (char)( !strcmp(buf[0], ".extern") || !strcmp(buf[1], ".extern") );
    if( external )
        external = EXTERN_LINE;
    else
        external = FALSE_VALUE;

    entry = (char)(!strcmp(buf[0], ".entry") || !strcmp(buf[1], ".entry"));
    if( entry )
        entry = ENTRY_LINE;
    else
        entry = FALSE_VALUE;

    if(isLabel( buf[0] ) && (entry || external))
    {
        printf("there is a Label \"%s\" in the beginning of extern or entry line\n", buf[0]);
        label = TRUE_VALUE;
    }
    if(external)
        return (char)(external + label);
    if(entry)
        return (char)(entry + label);

    return FALSE_VALUE;
}

int spaceForWords(char* line)
{
    char ** buffer = wordOfLine(line);
    char boolean;
    int space;  // count the space
    int index;

    if( (boolean = (char)isData(&buffer)) )   // check if data or string allocation is process is done in the line
    {
        switch (boolean)
        {
            case DATA_WITHOUT_LABEL:
                index = 1;  // start counting ageter .data, so it mean from 1 word.
                space  = 0;
                while( buffer[index++] != NULL )
                    ++space;
                freeWords(&buffer);
                return space;

            case DATA_WITH_LABEL:
                index = 2;  // start counting after the label and the .data so, it mean from 2 word.
                space = 0;
                while( buffer[index++] != NULL )
                    ++space;
                freeWords(&buffer);
                return space;

            case STRING_WITHOUT_LABEL:
                index = 1;  // start counting after .string, so it mean from 1 word.
                space = 0;
                while( buffer[index] != NULL )
                    space = space + ( (int)strlen(buffer[index++]) - 1 );  // I added the number of char that any given sting holds like "TalB" it's the length of the string decreed
                                                                  // by 1 is for the two quotation marks and one for the null terminator.
                freeWords(&buffer);
                return space;

            case STRING_WITH_LABEL:
                index  = 2; // start counting after the label and the .string, so it mean from 2 word.
                space = 0;
                while( buffer[index] != NULL )
                    space = space + (int)strlen(buffer[index++]) - 1 ;
                freeWords(&buffer);
                return space;
            default:
                printf("switch is spaceForWords function makes troubles\n");
        }
        /*  1. if data count the number of words in buffer after the .data
         *  2. if string sum the length of each string and decrease it by 2  till you reach null
         * */
    }

    if( ( isNextern(&buffer)) )
    {
        freeWords(&buffer);
        return 0;   // extern and entry are not requiring words
    }

    if( isDirctiveLine(&buffer) && isItLegalDirctiveLine(&buffer, (char)isLabel(buffer[0])) )
    {
        if(bothRegister(&buffer))
            return BOTH_REGISTER;   // if both operands are registers the space should be always 2

        index = isLabel( buffer[0] );   // if there is a label I want to start counting after the label, so I initial the index to be after the label.
        space = 0;
        while(buffer[index++] != NULL)  // in dirctive line you need word for the command and each word for addressing method
            ++space;
        freeWords(&buffer);
        return space;
    }

    freeWords(&buffer);
    return ERROR_VALUE;    /* in case the line is not valid the function will return ERROR_VALUE  */
}

int isDirctiveLine(char *** buffer)
{
    char ** buff = *buffer;
    Pips *tamp;
    char boolean = TRUE_VALUE;
    char label = FALSE_VALUE;     // label boolean value
    HashTable * saveWords = createHashTable();
    setup_Table(&saveWords);

    if(isLabel(buff[0]))    // check if the line have label a
        label = TRUE_VALUE;

    if( (tamp = search(&saveWords,buff[ label ] )) == NULL)     // I wrote buff[index + label] because if the line have label
        boolean = FALSE_VALUE;                                          // so i want to start read after the label, so I just added one.

    free(tamp); //  search return pointer to null so you need to free the memory in tamp
    free_table(&saveWords);
    return boolean;
}

char isItLegalDirctiveLine(char ***buffer, char ifLabel)
{
    char ** buff = *buffer;
    char * command;
    int boolean1;
    int boolean2;
                    /*  check if there is a label and if there is the code know to start from 1 index in buffer */
    if(ifLabel)
    {
        command = buff[1];
        ifLabel = TRUE_VALUE;   // sign into ifLabel 1
    }
    else
    {
        command = buff[0];
        ifLabel = FALSE_VALUE;   // sing into ifLabel 0
    }

    switch(inWhatGroup(command))
    {
        case GROUP_ONE:
            boolean1 = firstAddresing(buff[ifLabel + 1] ) || secondAddresing(buff[ ifLabel + 1] ) ||
                    thirdAddressing(buff[ ifLabel + 1]) || fourthAddresing(buff[ ifLabel + 1]);

            boolean2 = firstAddresing(buff[ifLabel + 2] ) || secondAddresing(buff[ ifLabel + 2] ) ||
                    thirdAddressing(buff[ ifLabel + 2]) || fourthAddresing(buff[ ifLabel + 2]);

            boolean1 = boolean1 && boolean2 && (buff[ifLabel +3] == NULL);
            break;

        case GROUP_TWO:
            boolean1 = firstAddresing(buff[ifLabel + 1]) || secondAddresing(buff[ifLabel + 1]) ||
                    thirdAddressing(buff[ifLabel + 1]) || fourthAddresing(buff[ifLabel + 1]);

            boolean2 = secondAddresing(buff[ifLabel + 2]) || thirdAddressing(buff[ifLabel + 2]) ||
                    fourthAddresing(buff[ifLabel + 2]);

            boolean1 = boolean1 && boolean2 && (buff[ifLabel + 3] == NULL);
            break;

        case GROUP_THREE:
            boolean1 = secondAddresing(buff[ifLabel + 1]) || thirdAddressing(buff[ifLabel + 1]) ||
                    fourthAddresing(buff[ifLabel + 1]);
            boolean2 = (buff[ifLabel + 2] == NULL);

            boolean1 = boolean1 && boolean2 ;
            break;

        case GROUP_FOUR:
            boolean1 = secondAddresing(buff[ifLabel + 1]) || thirdAddressing(buff[ifLabel + 1]);
            boolean2 = (buff[ifLabel + 2] == NULL);

            boolean1 = boolean1 && boolean2 && (buff[ifLabel + 3] == NULL);
            break;

        case GROUP_FIVE:
            boolean1 = (int)secondAddresing(buff[ifLabel + 1]);

            boolean2 = secondAddresing(buff[ifLabel + 2]) || thirdAddressing(buff[ifLabel + 2]) ||
                       fourthAddresing(buff[ifLabel + 2]);

            boolean1 = boolean1 && boolean2 && (buff[ifLabel + 3] == NULL);
            break;

        case GROUP_SIX:
            boolean1 = firstAddresing(buff[ifLabel + 1]) || secondAddresing(buff[ifLabel + 1]) ||
                       thirdAddressing(buff[ifLabel + 1]) || fourthAddresing(buff[ifLabel + 1]);

            boolean2 = ( (buff[ifLabel + 2]) == NULL );
            boolean1 = boolean1  && boolean2;
            break;

        case GROUP_SEVAN:
            boolean1 = (buff[ifLabel + 1] == NULL);
            break;

        case FALSE_VALUE:
            return FALSE_VALUE;
            break;
    }
    if(boolean1)
        return TRUE_VALUE;

    return FALSE_VALUE;
}

char inWhatGroup(char * command)
{   /*
 *      The group divided by what Addressing methods you can apply on the Command word
 *      you can see by the table which Addressing methods each group have
 *         _______________________________________________________________________________
 *         |Command Word    |   Source Operand    |   Destination Operand   |   Group    |
 *         |----------------|--------------------–|––––---------------------|------------|
 *         |       cmp      |      0,1,2,3        |         0,1,2,3         |  GROUP_ONE |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       mow      |      0,1,2,3        |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|            |
 *         |       add      |      0,1,2,3        |         1,2,3           |  GROUP_TWO |
 *         |----------------|---------------------|-------------------------|            |
 *         |       sub      |      0,1,2,3        |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       clr      |      *Nothing*      |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|            |
 *         |       not      |      *Nothing*      |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|            |
 *         |       inc      |      *Nothing*      |         1,2,3           | GROUP_THREE|
 *         |----------------|---------------------|-------------------------|            |
 *         |       dec      |      *Nothing*      |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|            |
 *         |       red      |      *Nothing*      |         1,2,3           |            |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       jsr      |      *Nothing*      |         1,2             |            |
 *         |----------------|---------------------|-------------------------|            |
 *         |       jmp      |      *Nothing*      |         1,2             |  GROUP_FOUR|
 *         |----------------|---------------------|-------------------------|            |
 *         |       bne      |      *Nothing*      |         1,2             |            |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       lea      |          1          |         1,2,3           | GROUP_FIVE |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       prn      |      *Nothing*      |         0,1,2,3         | GROUP_SIX  |
 *         |----------------|---------------------|-------------------------|------------|
 *         |       rts      |      *Nothing*      |        *Nothing*        |            |
 *         |----------------|---------------------|-------------------------| GROUP_SEVAN|
 *         |      stop      |      *Nothing*      |        *Nothing*        |            |
 *         |________________|_____________________|_________________________|____________|
 *
 * */
    if( !strcmp(command,"cmp") )
        return GROUP_ONE;
    if( !strcmp(command,"mov") )
        return GROUP_TWO;
    if( !strcmp(command,"add") )
        return GROUP_TWO;
    if( !strcmp(command,"sub") )
        return GROUP_TWO;
    if( !strcmp(command,"clr") )
        return GROUP_THREE;
    if( !strcmp(command,"not") )
        return GROUP_THREE;
    if( !strcmp(command,"inc") )
        return GROUP_THREE;
    if( !strcmp(command,"dec") )
        return GROUP_THREE;
    if( !strcmp(command,"red") )
        return GROUP_THREE;
    if( !strcmp(command,"jsr") )
        return GROUP_FOUR;
    if( !strcmp(command,"jmp") )
        return GROUP_FOUR;
    if( !strcmp(command,"bne") )
        return GROUP_FOUR;
    if( !strcmp(command,"lea") )
        return GROUP_FIVE;
    if( !strcmp(command,"prn") )
        return GROUP_SIX;
    if( !strcmp(command,"rts") )
        return GROUP_SEVAN;
    if( !strcmp(command,"stop") )
        return GROUP_SEVAN;
    return FALSE_VALUE;
}




char firstAddresing(char* word)
{
    char * tamp = word;
    char *endptr;
    char boolean = FALSE_VALUE;
    int number;

    if(tamp[0] == '#')
    {
        boolean = TRUE_VALUE;
        tamp++;
    }
    else
        return boolean;

    number = (int) strtol(tamp, &endptr, 10);
    if(*endptr == '\0')  // this mean the tamp hold only number after # sing
        return TRUE_VALUE;

    return FALSE_VALUE;
}
char secondAddresing(char* word)
{
    char aStart  = 'A';
    char aEnd  = 'Z';
    char bStart = 'a';
    char bEnd = 'z';
    char cStart = '0';
    char cEnd = '9';
    char * tamp = word;
    int index;

    if( fourthAddresing(word) ) //  if it also can be the third addressing
        return FALSE_VALUE;

    for(index = 0; index< SIZE_OF_LABEL && tamp[index] != '\0'; index++)
    {
        if( !( (cStart <= tamp[index] && tamp[index] <= cEnd) || (bStart <= tamp[index] && tamp[index] <= bEnd) || (aStart <= tamp[index] && tamp[index] <= aEnd) ) )
            return FALSE_VALUE;
    }
    return TRUE_VALUE;
}
char thirdAddressing(char* word)
{
    int index;
    char array[8][4] = {"*r0","*r1","*r2",
                        "*r3","*r4","*r5",
                        "*r6","*r7"};
    for(index = 0; index < REGISTER_NUM; index++)
        if( !strcmp(word, array[index]))
            return TRUE_VALUE;
    return FALSE_VALUE;
}
char fourthAddresing(char* word)
{
    int index;
    char array[8][3] = {"r0","r1","r2",
                        "r3","r4","r5",
                        "r6","r7"};
    for(index = 0; index < REGISTER_NUM; index++)
        if(!strcmp(word, array[index]))
            return TRUE_VALUE;
    return FALSE_VALUE;
}

char whatAddresing(char *address)
{
    if(firstAddresing(address))
        return FIRST_ADDRESING_METHOD;
    if(secondAddresing(address))
        return SECOND_ADDRESING_METHOD;
    if(thirdAddressing(address))
        return THIRD_ADDRESING_METHOD;
    if(fourthAddresing(address))
        return FORTH_ADDRESSING_METHOD;

    return FALSE_VALUE;
}

char bothRegister(char *** buffer)
{
    int isLabe;
    char boolean1 = FALSE_VALUE;
    char ** buf = *buffer;

    isLabe = isLabel(buf[0]);
    if(buf[isLabe +2] == NULL)
        return FALSE_VALUE;

    boolean1 = (char)(boolean1 || ( thirdAddressing(buf[isLabe +1]) && thirdAddressing(buf[isLabe + 2]) ));
    boolean1 = (char)(boolean1 || ( thirdAddressing(buf[isLabe +1]) && fourthAddresing(buf[isLabe + 2]) ));
    boolean1 = (char)(boolean1 || ( fourthAddresing(buf[isLabe +1]) && thirdAddressing(buf[isLabe + 2]) ));
    boolean1 = (char)(boolean1 || ( fourthAddresing(buf[isLabe +1]) && fourthAddresing(buf[isLabe + 2]) ));

    return boolean1;
}

char * toOct( int address )
{
    char *tamp = (char * )malloc(SIZE_OF_OCT);
    char start = '0';
    int rest, index;
    memset(tamp, '0',SIZE_OF_OCT);

    tamp[SIZE_OF_OCT - 1] = '\0';
    index = SIZE_OF_OCT - 2;
    while(address != 0)
    {
        rest = address % 8;
        tamp[index] = (char)(rest + start);
        address = address / 8;
        index--;
    }
    return tamp;
}

char ** binarToOct(Pips **DIC, int len)
{
    Pips *Ic = *DIC;
    char ** buffer = (char**)malloc(STORAGE_LIMIT* sizeof(char *));
    int indexPips,index ,tamp;
    Pips tester = {0b111};
    spPips belowZero;
    char start = '0';

    indexPips = 0;
    while(indexPips <= len )
    {
        if(Ic[START_OF_IC + indexPips].value < 0)
        {
            belowZero.value = (unsigned int)Ic[START_OF_IC + indexPips].value;
        }

        buffer[indexPips] = (char*)malloc(SIZE_OF_OCT);
        for (index = 0; index < SIZE_OF_OCT; index++)
        {
            if(Ic[START_OF_IC + indexPips].value < 0)
                tamp = ((belowZero.value & tester.value) >>(index *3));
            else
                tamp = ((Ic[START_OF_IC + indexPips].value & tester.value) >> (index * 3));

            if (0b000 <= tamp && tamp <= 0b0111)
                tamp = (tamp + start);
            else
                printf("There is a problem with binaryToOct!!!");

            buffer[indexPips][SIZE_OF_OCT - index -2] = (char)tamp;   // you enter the number starting from the end to the beginning
            tester.value = tester.value << 3;
        }
        buffer[indexPips][SIZE_OF_OCT-1] = '\0';
        indexPips++;
        tester.value = 0b0111;
    }
    return buffer;
}

void freeOct(char *** buffer)
{
    char ** buff = *buffer;
    int index;
    for(index = 0; index<STORAGE_LIMIT && buff[index] != NULL; index++)
    {
        free(buff[index]);
    }
    free(buff);
}
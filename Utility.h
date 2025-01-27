//
// Created by Tal on 29/08/2024.
//

#ifndef THEPROGECT_UTILITY_H
#define THEPROGECT_UTILITY_H
#include "dataBase.h"

#define DATA_WITHOUT_LABEL  1
#define DATA_WITH_LABEL 2
#define STRING_WITHOUT_LABEL  3
#define STRING_WITH_LABEL  4
#define EXTERN_LINE 2
#define ENTRY_LINE  4

char** wordOfLine( char* line);    // take line and write each word from the line to an array cell each cell contain one word
void freeWords(char *** buffer);

int isData(char *** buff);
char isNextern(char *** buffer);  // is the line contain extern or entry

int spaceForWords(char* line);  // Decide how much word the line will take
    int isDirctiveLine(char *** buffer);
    char isItLegalDirctiveLine(char ***buffer, char ifLabel); // Boolean function that check whether the line is legal statement
        char inWhatGroup(char * command);
        char firstAddresing(char* word);        // check whether it is the first addressing method
        char secondAddresing(char* word);   //  check whether it is the second addressing method
        char thirdAddressing(char* word);   //  check whether it is the third addressing method
        char fourthAddresing(char* word);       //  check whether it is the fourth addressing method
char whatAddresing(char *address);     // get operand and decide what addressing method
char bothRegister(char *** buffer);  // check if the both operand are registers
char * toOct( int address);
char ** binarToOct(Pips **DIC, int len);
void freeOct(char *** buffer);

#endif //THEPROGECT_UTILITY_H

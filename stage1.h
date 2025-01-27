//
// Created by Tal on 06/09/2024.
//

#ifndef THEPROGECT_STAGE1_H
#define THEPROGECT_STAGE1_H

#include "dataBase.h"
#define VAR_STARTING_POINT 100
#define VARIABLE_VALUE_LIMIT 16383 // 2^14 - 1 = 16383 which is the limit of the numbers
#define STORAGE_FOR_EACH 2048
#define STORAGE_LIMIT 1948 // 4096 is the whole storage in the computing component so 2048 for each section and 2048-100 = 1948
#define EXTERNAL_ADDRESS 0
#define FIRST_ADDRESING_METHOD 1
#define SECOND_ADDRESING_METHOD 2
#define THIRD_ADDRESING_METHOD 3
#define FORTH_ADDRESSING_METHOD 4
#define COUNTERS_AMOUNT 2   // stand for the ic and dc counters
#define C_IC 0
#define C_DC 1


int * stage1(char*file, Pips ** IC, Pips **DC, Nextern **externalTable , LabelNode **labelList);

int isLabel(char* line);
int enterData(Pips ** DC, int variable, int dcCounter);
int enterString(Pips **DC, char * string, int dcCounter);
Pips createPips(char *word, char label, int space);
void flipToCode( LabelNode **labelList, int ic);

void makeCode(LabelNode **head, int IC );

#endif //THEPROGECT_STAGE1_H

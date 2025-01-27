//
// Created by Tal on 15/10/2024.
//

#ifndef THEPROGECT_STAGE2_H
#define THEPROGECT_STAGE2_H
#include "dataBase.h"

#define SOURCE_PLACE_REGISTER 1
#define DESTNTION_PLACE_REGISTER 2
#define TOW_WORDS_DEST 0
#define THREE_WORDS_SOURCE 1
#define THREE_WORDS_DEST 2

int stage2(char *file, Pips **IC, LabelNode ** labelList, Nextern **NexternList);
void enterFirstAdressing(Pips **IC, int ic, char* operand);
void enterSecondAdressing(Pips **IC, LabelNode **List, int ic, char* name);
void enterThirdAdressing(Pips **IC, int ic, char* operand, char state, char ifBothRegister);
void enterFourthAdressing(Pips **IC, int ic, char* operand, char state, char ifBothRegister);

#endif //THEPROGECT_STAGE2_H

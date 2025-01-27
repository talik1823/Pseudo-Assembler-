//
// Created by Tal on 19/08/2024.
//

#ifndef PREASSEMBLY_H
#define PREASSEMBLY_H

#include "constantVar.h"
#include "preAssembly.h"
#include "Utility.h"
void preAssembly(char* fileName);
FILE *macrOn(FILE **oldFile,macrType1 **table, char ***buffer, char * macrName);


#endif //PREASSEMBLY_H

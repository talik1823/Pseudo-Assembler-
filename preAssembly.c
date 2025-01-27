//
// Created by Tal on 24/07/2024.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dataBase.h"
#include "constantVar.h"
#include "preAssembly.h"
#include "Utility.h"


void preAssembly(char* fileName)
{
    line ffileName;
    strcpy(ffileName, fileName);
    FILE * oldFile = fopen(strcat(ffileName,".as"), "r");
    FILE * newFile = fopen("replace.tmp", "w");

    char flagMacr = FALSE_VALUE;
    line tamp, macrName;
    macrType1 *table = setMacr1Node(); // create macr table
    macrType1 *name;
    macrType2 * ptr;
    char **buffer;

    if( oldFile == NULL || newFile == NULL )
    {
        printf("\nUnable to open the file\n");
        printf("please check if files exist\n");
        return;
    }

    while( fgets(tamp, sizeof(tamp), oldFile) != NULL ) {
        buffer = wordOfLine(tamp);



        if (/* 1 */buffer[0] == NULL || /* 2 */buffer[0][0] == ';')   // 1. if the line is empty or have only white spaces
            continue;                                 // 2. or if the line is note in the code.

        if ( ( name = searchName(&table, buffer[0]) ) != NULL )   // NOTE: make a function of that | Stage 2
        {
             ptr = name->next;

            while(ptr->next != NULL)
            {
               fputs(ptr->line, newFile);
               ptr = ptr->next;
            }
            fputs(ptr->line, newFile);
            continue; //    return to the start of the while loop
        }

        if( !strcmp("macr", buffer[0]) )    // start of macr  declaration
        {
            flagMacr = TRUE_VALUE;
            strcpy(macrName, buffer[1]);
            setMacrName(macrName, &table);
        }

        if(flagMacr)    /*    the function is during declaration proses of macr (FILE oldFile, **table, char **buffer     */
        {
            oldFile = macrOn(&oldFile, &table, &buffer, macrName);
            flagMacr = FALSE_VALUE;
            continue;
        }
        /*  if the function reach this place this mean this the line is not macr and it is just a rugular line of code   */
        fputs(tamp, newFile);
        free(buffer);
    }
    freeMacrTable(&table);
    fclose(oldFile);
    fclose(newFile);
    rename("replace.tmp", strcat(strcpy(ffileName, fileName) , ".am") );

}


FILE *macrOn(FILE **oldFile,macrType1 **table, char ***buffer, char * macrName)
{
    FILE *old = *oldFile;
    char flagMacr = TRUE_VALUE;
    line tamp;  // temporary line
    char **buf = *buffer;

    while(flagMacr)
    {
        if (!strcmp("macr", buf[0]))// if start of macr
        {
            fgets(tamp, sizeof(tamp), old);
            freeWords(&buf);
            buf = wordOfLine(tamp);
        }

        if (!strcmp("endmacr", buf[0])) // if end of macr
        {
            flagMacr = FALSE_VALUE;
            freeWords(&buf);
            continue;
        }
        /*  if the function reach this place this mean the code in the middle of the macr declaration   */

        setMacrLine(macrName, tamp, table);
        fgets(tamp, sizeof(tamp), old);
        freeWords(&buf);
        buf = wordOfLine(tamp);
    }

    return old;
}




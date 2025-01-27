//
// Created by Tal on 24/07/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataBase.h"
#include "constantVar.h"
#include "Utility.h"
#define SIZE_OF_NAME_MACR 76    // 81 is the maximum line inculde \n so "macr " is 5 bit 81-5 = 76



macrType1* setMacr1Node()   // this function used to create the table and also use by setMacrName function to create empty Node
{
    macrType1 *head = (macrType1*)malloc(sizeof(macrType1));

    if(head == NULL)
    {
        printf("there is an issue: |23| cannot allocate memory for macr \n");
        return NULL;    /*NOTE: to do something with this NULL return */
    }

    memset(head->name,'\0',SIZE_OF_LINE);// put \0 across all the line
    head->contine = NULL;
    head->next = NULL;
    return head;
}

macrType2 *setMacr2Node(line line)
{
    macrType2 * node = (macrType2*)malloc(sizeof(macrType2));
    int length = (int) strlen(line);

    if(node == NULL)
    {
        printf("Error: node can't allocate space |2| dataBase.c/setMacr2Node\n");
        return NULL;    /*NOTE: to do something with this NULL return */
    }
    if(line[length - 1] != '\n')    // save the line with new line char
    {
        line[length] = '\n';
        line[length + 1] = '\0';    // length + 1 is still in the size that has been allocated for line
    }

    strcpy(node->line,line);
    node->next =NULL;
    return node;
}

void setMacrName(line name, macrType1 **head)
{
    macrType1 *tamp = *head;

    if(name[ strlen(name)-1 ] == '\n')     // remove the new line character from the name
        name[ strlen(name) -1 ] = '\0';

    while( *tamp->name != '\0' )
    {
        if( tamp->contine != NULL )
            tamp = tamp->contine;
        else
        {
            tamp->contine = setMacr1Node();
            tamp = tamp->contine;
        }
    }
    strcpy( tamp->name,name );
    tamp->contine = NULL;
    tamp->next = NULL;
}

void setMacrLine(line name, line line, macrType1**head) // Importent: adding line of source code macr that already exist
{
    macrType1 *tamp = *head;
    macrType2 *tampLine;



    while( strcmp(tamp->name, name) != FALSE_VALUE )     //skip on all the other names
    {
        if(tamp->contine != NULL)
            tamp = tamp->contine;
        else
        {
            printf("Erorr: the name is incorrect in char| dataBase.c/setMacrLine/1\n");
            return;
        }
    }

    if(!strcmp(tamp->name, name))
    {
        if( tamp->next == NULL )
            tamp->next = setMacr2Node(line);

        else{
            tampLine = tamp->next;
            while (tampLine->next != NULL)
                tampLine = tampLine->next;

            tampLine->next = setMacr2Node(line);
            }
    }
}

macrType1 *searchName(macrType1 **head,line name)/*    the functoin return NULL if there is no name  and return macrType1 if the name exsist*/
{
    macrType1 *tamp = *head;

    if(name[ strlen(name) -1 ] == '\n') // remove the new line char
        name[ strlen(name) -1 ] = '\0';

    if( *tamp->name == '\0' )   //first digit in first name is \0
        return NULL;    // the table is empty


    while( strcmp(tamp->name, name) != FALSE_VALUE )
    {
        if(tamp->contine == NULL)
            return NULL;    // the list is not contain the name
        else
            tamp = tamp->contine;
    }
    return tamp;
}

void freeMacrTable(macrType1 **head)
{
    macrType1 *tamp  = *head;
    macrType1 *tamp1;

    while( tamp != NULL )
    {
        tamp1 = tamp->contine;
        freeMacrName( &(tamp->next) );
        free(tamp);
        tamp = tamp1;
    }
}

void freeMacrName( macrType2 **head )
{
    macrType2 *first = *head;
    macrType2 *tamp = *head;

    while( tamp !=NULL )
    {
        tamp = first->next;
        free(first);
        first = tamp;
    }
}

void printMacrToFile( FILE **toWrite, macrType1 **name )    //  NOTE: print the Macr content without deleting  the Macr calling
{
        macrType1 *tamp = *name;
        macrType2 *ptr;

        if( tamp->next == NULL )
        {
            printf( "the name %s not contain any data of Macr\n", tamp->name );
            return;
        }

        ptr = tamp->next;

        while( ptr != NULL )
        {
            fputs(ptr->line, *toWrite);
            ptr = ptr->next;
        }
}

//*******************************************************************************************************************************
//  functions for the linked list that store external and internal label location in the source file.
//
//*******************************************************************************************************************************

Nextern *creatExternIntryList()
{
    Nextern *head = (Nextern*) malloc(sizeof(Nextern));
    memset(head->name,'\0',SIZE_OF_LABEL);
    head->address = NEUTRAL_VALUE_ADDRESS;
    head->external = FALSE_VALUE;
    return head;
}

void setName(Nextern **head, label name, char external)
{
    Nextern *tamp = *head;

    if( name[strlen(name)-1] == '\n' )
        name[strlen(name)-1] = '\0';

    if(tamp->name[0] == '\0')
    {
        strcpy(tamp->name, name);
        tamp->external = external;
        tamp->address = NEUTRAL_VALUE_ADDRESS;
        return;
    }

    if(!strcmp(tamp->name, name))
    {
        printf("|1| there is an already %s label", name);
        return;
    }

    while(tamp->contine != NULL && strcmp(tamp->name, name) != FALSE_VALUE)
        tamp = tamp->contine;

    if(!strcmp(tamp->name, name))
    {
        printf("|2| there is an already %s label\n", name);
        return;
    }

    tamp->contine = allocatenode(name,NEUTRAL_VALUE_ADDRESS, external);
}

Nextern *allocatenode(label name, int address, char external)
{
    Nextern *tamp = (Nextern *) malloc(sizeof(Nextern));

    if(tamp == NULL)
    {
        printf("|10| there is problem in allocate label Node\n");
        return NULL;
    }

    strcpy( tamp->name, name );
    tamp->address = address;
    tamp->external = external;
    return tamp;
}

char thereIsName( Nextern **head, label name)
{
    Nextern *tamp = *head;
    char flag = FALSE_VALUE;
    int leangth;

    if( name[leangth = (int)((strlen(name) - 1))] == ':')
        name[leangth] = '\0';

    while(tamp != NULL)
    {
         if( !strcmp( tamp->name, name ) )
               flag = TRUE_VALUE;

         tamp = tamp->contine;
    }
    return flag;
}

void addAddress( Nextern **head, label name, int address )
{
    Nextern *tamp = *head;
    char flag = TRUE_VALUE;   // it is char to reduce byte usage
    int leangth;

    if( name[leangth = (int)(strlen(name) - 1)] == ':')
        name[leangth] = '\0';

    while( tamp != NULL && flag )
    {
        if( !strcmp(tamp->name, name ))
            flag = FALSE_VALUE;
        else
            tamp = tamp->contine;
    }

    if(tamp == NULL)
    {
        printf("2 |there is no %s label\n ", name);
        return;
    }

    if(tamp->address == NEUTRAL_VALUE_ADDRESS)
    {
        tamp->address = address;
        return;
    }

    while( tamp->next != NULL  )
        tamp = tamp->next;

    if(tamp->next == NULL)
        tamp->next = allocatenode(name, address, ERROR_VALUE);  // when we add address there is no reason to mention the  if it entry or external

}

void makeFile(Nextern **head, char mode, line file)    //mode: external or internal line: the name of the file without .as external == 'e' internal == 'i'
{
    line fFile, file2;
    FILE *newptr;
    Nextern *ptrName = *head;
    Nextern *tamp1, *tamp;
    char *octNum;
    line tampline;

    strcpy(fFile,file);
    newptr = fopen ( strcat(fFile,".tmp"), "w" );

    if(newptr == NULL)
    {
                /*unable to create a file */
        printf(" |1|cannot create new file |function: makeFile\n ");
        return;
    }


        tamp = ptrName;
        if(mode == 'e')  // the case of creating extern
        {
            while(tamp != NULL)
            {
                if( tamp->external )
                {
                    tamp1 = tamp;
                    while(tamp1 != NULL)
                    {
                        octNum = itoa(tamp1->address);
                        strcpy(tampline, tamp1->name);
                        strcat(tampline,"     ");
                        strcat(tampline, octNum);
                        strcat(tampline, "\n");
                        fputs(tampline,newptr);
                        tamp1 = tamp1->next;
                        free(octNum);
                    }
                }
                    tamp = tamp->contine;
            }
        }

        if(mode == 'i') // the case of creating entry
        {
            while(tamp != NULL)
            {
                if( tamp->external == FALSE_VALUE )
                {
                    tamp1 = tamp;
                    while(tamp1 != NULL)
                    {
                        octNum = itoa(tamp1->address);
                        strcpy(tampline, tamp1->name);
                        strcat(tampline,"     ");
                        strcat(tampline, octNum);
                        strcat(tampline, "\n");
                        fputs(tampline,newptr);
                        tamp1 = tamp1->next;
                    }
                }
                    tamp = tamp->contine;
            }
        }

    fclose(newptr);
    strcpy(file2, file);
    if(mode == 'e')
        rename(fFile, strcat(file2,".ext"));

    else if(mode == 'i')
        rename(fFile, strcat(file2,".ent"));

}

char* itoa(int number )      /* design only for number with 4 digit and less */
{
    char * ptr = (char*)malloc(SIZE_OF_ADDRESS);
    char tamp;
    int index = SIZE_OF_ADDRESS - 2;// starting from the second from the end to leave space for '\n'
    char start = '0';
    memset(ptr, '0', SIZE_OF_ADDRESS - 1);
    ptr[SIZE_OF_ADDRESS-1] = '\0';

    while( number != 0 && index >= 0)
    {
        tamp = (char)(number % 10);
        number = number / 10;
        *(ptr + index) = (char)(tamp + start);
        index--;
    }

    if(index != 0)
        while(index != 0)
            *(ptr + index--) = '0';
    return ptr;
}

void freeNexternList(Nextern **head)
{
    Nextern *first = *head;
    Nextern *tamp1;
    Nextern *tamp2;

    while( first != NULL )
    {
        tamp1 = first;
        tamp2 = first;
        first = first->contine;

        while( tamp1 != NULL )
        {
            tamp1 = tamp1->next;
            free(tamp2);
            tamp2 = tamp1;
        }
    }
}

/******************************************************************************************
 * functions for the linked list that store all the labels the code contain
 *
 * ****************************************************************************************/

LabelNode *creatLabelTableList()
{
    LabelNode *head = (LabelNode *) malloc(sizeof(LabelNode));
    memset(head->name, '\0', SIZE_OF_LABEL);
    return head;
}

void setLabel(LabelNode **head, label name, int address, char data, char relocatable, int DataLength)
{
    LabelNode *tamp = *head;
    int length = (int)strlen(name);

    if( name[length - 1 ] == ':' )
        name[length -1] = '\0';

    if(tamp->name[0] == '\0')
    {
        strcpy(tamp->name, name);
        tamp->address = address;
        tamp->Data = data;
        tamp->Relocatable = relocatable;
        tamp->DataLength = DataLength;
        tamp->Entry = FALSE_VALUE;
        return;
    }

    while(tamp->next != NULL && strcmp(tamp->name, name) != FALSE_VALUE)
        tamp = tamp->next;

    if(!strcmp(tamp->name, name))
    {
        printf("there is already and name %s in the labeling table\n",tamp->name);
        return;
    }

    tamp->next = mallocLabelNode(name, address, data, relocatable, DataLength);
}

char nameExist(LabelNode **head, char * name)
{
    LabelNode * tamp = *head;
    char boolean = FALSE_VALUE;
    int length;

    if(name[length = (int)(strlen(name) - 1) ] == ':')
        name[length] = '\0';
    while(tamp != NULL && ( boolean == FALSE_VALUE ))
    {
        if (!strcmp(tamp->name, name))
            boolean = TRUE_VALUE;
        tamp = tamp->next;
    }

    return boolean;
}

LabelNode *mallocLabelNode( label name, int address, char data, char relocatable, int DataLength)
{
    LabelNode *tamp = malloc(sizeof (LabelNode));

    if(tamp == NULL)
    {
        printf("|5| there is a problem with allocate memory to Node from the labeling table\n");
        return NULL;
    }
    strcpy(tamp->name, name);
    tamp->address = address;
    tamp->Data = data;
    tamp->Relocatable = relocatable;
    tamp->DataLength = DataLength;
    tamp->Entry = FALSE_VALUE;

    return tamp;
}

int getAddress(LabelNode **head, label name)
{
    LabelNode *tamp = *head;

    while( tamp != NULL )
    {
        if (!strcmp(tamp->name, name) )
            return tamp->address;

        else
            tamp = tamp->next;
    }
    printf("the name %s is not contained in the labeling table\n", name);
    return ERROR_VALUE;
}

int makeDataCode(LabelNode **head, int IC)  // NOTE: the function should get IC address that pointing to an empty space in the code
{
    LabelNode *tamp = *head;

    while( tamp != NULL )
    {
        if( tamp->Data )     // mean that the node is in the data section
        {
            tamp->Data = FALSE_VALUE;
            tamp->address = START_OF_IC + IC++;   // forwarded the IC indicator for the next label
                                                // and to return IC which indicate for new space
        }
        tamp = tamp->next;

    }
    return IC;
}

void freeLabelTableList(LabelNode **head)
{
    LabelNode *tamp = *head;
    LabelNode *ptr = *head;

    while(tamp != NULL)
    {
        tamp = tamp->next;
        free(ptr);
        ptr = tamp;
    }
}

void makeEntry(LabelNode **head, label name)
{
    LabelNode *tamp = *head;
    char boolean1 = TRUE_VALUE;

    while(tamp != NULL && boolean1)
    {
        if(!strcmp(name, tamp->name))
        {
            tamp->Entry = TRUE_VALUE;
            boolean1 = FALSE_VALUE;
        }
        tamp = tamp->next;
    }
}
/*
 *  functions of hash map the hash map used in this system in order to organize all the
 *  save word of the compiler, it's the fastest way to reach the code of each
 *  save word.
 */

            /*    simple hash code by the deviation method    */
unsigned int hash(char *key)
{
    char *tamp = key;
    int index;
    int sum = 0;
    int limter = 0;

    while(*tamp != '\0' && limter++ < SAVE_WORD_SIZE )
        sum = sum + (int)(*(tamp++));     // sum all the save word and save the total

    index = sum % TABLE_SIZE;   // mod the sum to the size of the hashTable
    return index;
}

HashTable* createHashTable()
{
    HashTable *table = (HashTable*) malloc(sizeof(HashTable));
    table->buckets = (HashNode **) malloc( sizeof(HashNode) * TABLE_SIZE );
    int index;

    for(index = 0 ; index<TABLE_SIZE;index++)
    {
        table->buckets[index] = NULL;
    }
    return table;
}


void insert(HashTable **table, char* name, Pips *sNumber)
{
    HashTable *ptr = *table;
    HashNode *inserted;
    HashNode *tamp;
    unsigned int index;
                          /*  creating the node with the given parameter   */
    inserted = (HashNode *) malloc( sizeof( HashNode ));
    inserted->value.value = sNumber->value;
    strcpy(inserted->name, name);
    inserted->next = NULL;
                        /*  inserting the node to the correct place    */
    index = hash(name);
    if(ptr->buckets[index] == NULL)
        ptr->buckets[index] = inserted;
    else
    {
        tamp = ptr->buckets[index];
        while( tamp->next != NULL )
            tamp = tamp->next;
        tamp->next = inserted;
    }
}

void setup_Table(HashTable **table)
{
    HashTable *tamp = *table;

                        /*  sing for the operands */
    insert(&tamp,"mov",&mov);
    insert(&tamp, "cmp", &cmp);
    insert(&tamp,"add",&add);
    insert(&tamp,"sub",&sub);
    insert(&tamp,"lea",&lea);
    insert(&tamp,"clr",&clr);
    insert(&tamp,"not",&not);
    insert(&tamp,"inc",&inc);
    insert(&tamp,"dec",&dec);
    insert(&tamp,"jmp",&jmp);
    insert(&tamp,"bne",&bne);
    insert(&tamp,"red",&red);
    insert(&tamp,"prn",&prn);
    insert(&tamp,"jsr",&jsr);
    insert(&tamp,"rts",&rts);
    insert(&tamp,"stop",&stop);

                    /*  sign for the load and linking stage */
    insert(&tamp,"a",&a);
    insert(&tamp,"r",&r);
    insert(&tamp,"e",&e);

                    /*  sign for the addressing methods source and destination  */
    insert(&tamp,"d_zero",&d_zero);
    insert(&tamp,"d_one",&d_one);
    insert(&tamp,"d_two",&d_two);
    insert(&tamp,"d_three",&d_three);

    insert(&tamp, "s_zero", &s_zero);
    insert(&tamp, "s_one", &s_one);
    insert(&tamp, "s_two", &s_two);
    insert(&tamp, "s_three", &s_three);

}

Pips *search(HashTable **table, char *name)
{
    HashTable *tamp = *table;
    HashNode *node;
    unsigned int code;
    Pips *ptr = malloc(sizeof(Pips));

    code = hash(name);
    node = tamp->buckets[code];
    while(node->next != NULL && strcmp(name, node->name) != FALSE_VALUE)
        node = node->next;

    if(node->next == NULL && strcmp(name,node->name) != FALSE_VALUE)
    {
        // printf("Eror|1|:there is no %s in the hash table\n",name);
        return (Pips*) NULL;
    }

    ptr->value = node->value.value;
    return ptr;
}

void free_table(HashTable **table)
{
    HashTable *tamp = *table;
    int i;
    for( i = 0; i < TABLE_SIZE; i++)
        free_Node(&tamp->buckets[i]);
}

void free_Node(HashNode **node)
{
    HashNode *ptr1 = *node;
    HashNode *ptr2 = ptr1;

    while(ptr1 != NULL)
    {
        ptr1 = ptr1->next;
        free(ptr2);
        ptr2 = ptr1;

    }
}


//void Pipscpy(Pips *dest, const Pips *source)
//{
//    dest
//}
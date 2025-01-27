//
// Created by Tal on 24/07/2024.
//
#ifndef DATABASE_H
#define DATABASE_H
#include "stdio.h"

#define SIZE_OF_LINE 82     // the maximum line is 80 without \n and \0 so 80+2 = 82
#define SIZE_OF_LABEL 32    // the maximum length of label and also the ':' char
#define BIT_NUMBER 15
#define TRUE_VALUE 1
#define FALSE_VALUE 0
#define NEUTRAL_VALUE_ADDRESS 0
#define DIGITS_OF_LOCATIONS 5    //the length of location address in digit
#define ERROR_VALUE (-1)
#define START_OF_IC 100
#define SAVE_WORD_SIZE 10    // the longest operation is "stop"
#define TABLE_SIZE 41 // Prime number and good for hashing
#define SIZE_OF_OCT 6   // the length is five but with the null terminator it will be six;
#define SIZE_OF_ADDRESS 5   // 4 digit with \0


typedef char line[SIZE_OF_LINE];
typedef char label[SIZE_OF_LABEL];
typedef char saveWord[SAVE_WORD_SIZE];

typedef struct Pips
{
    int value: BIT_NUMBER;
}Pips;

typedef struct spPips   // special type of pips the in use in places you need to relate to Pips in unsigned manner
{
    unsigned int value: BIT_NUMBER;
}spPips;
typedef struct macrType2
{
    line line;
    struct macrType2 *next;
}macrType2;

typedef struct macrType1
{
    line name;
    struct macrType1 *contine;
    macrType2 *next;
}macrType1;
                                                                    //              ->contine           ->contine              ->contine
typedef struct Nextern   // Node of External & Internal locations       name, address –  >   name, address - >  name, address -   > .   .   .
{//                                                                           | ->next             | ->next           |
    label name;     //                                                        ⌄                    ⌄                  ⌄
    int address;    //                                                  name, address        name, address            .
    char external;  // if true the label is external otherwise internal
    struct Nextern *next;   // move to the next address with the same name
    struct Nextern *contine;//  move to the next name                         |  ->next            | ->next           .
}Nextern;   //                                                                ⌄                    ⌄                  .
//                                                                      name, address        name, address


/*
 * LabelNode: Node for the list that operate the lable table data
 */

typedef struct LabelNode
{
    label name;

    int address;    // the address of the label in the code or in Data (depend on data boolean value)

    char Data;      // if the value is 0 the label is currently in the code section  and if the value is different then
                    // 0 the label is in the data section

    char Relocatable;   // if the value is 0 so the node indicate for extern label. and if the value is different then
                        // 0 the label is relocatable.

    int DataLength;    // if the Label is data so this parameter contain the number of words the data label allocate
                       // if this is a code Label the value is irrelevant and be ERROR_VALUE
    char Entry;
    struct LabelNode *next;
}LabelNode;


typedef struct HashNode
{
    saveWord name;
    Pips value;
    struct HashNode *next;
}HashNode;

typedef struct HashTable
{
    HashNode **buckets;
}HashTable;


/***************************************************************************************************************************************
 * All the function that take care the Macro list
 *
 * macrType1: is contain the name of the macr and pointer the lines the Macro contain (*next), and pointer to the next name (*contine)
 * macrType2: is contain the line store in Macros statment and pointer to the next line if it exist (*next)
 * *************************************************************************************************************************************/

macrType1 *setMacr1Node(void);//     return pointer to frash memory  node to "macrType1"
macrType2 *setMacr2Node(line str);
void setMacrName(char *name,macrType1 **head);   //set the name before write all the macro
void setMacrLine(char *name, char *line, macrType1**head);
macrType1 *searchName(macrType1 **head,line name);
void freeMacrTable(macrType1 **head);
void freeMacrName(macrType2 **head);
void printMacrToFile(FILE **toWrite, macrType1 **name);

/*
 * those function take care in the external and the internal Labels list
 * */
Nextern *creatExternIntryList(void);
void setName(Nextern **head, label name,char external);
Nextern *allocatenode(label name, int address,char external);
char thereIsName(Nextern **head, label name);   // check if the list contain the name send to the function
void addAddress(Nextern **head ,label name,int address);
void makeFile(Nextern **head,char mode, line file);
char * itoa(int number );    // transfer int with maximum 4 digit to string
void freeNexternList(Nextern **head);

/*
 * these function deal with the linked list of the labels table
 *
 * */
LabelNode *creatLabelTableList(void);   // using for the first creation of the list make list without name declaration
void setLabel(LabelNode **head, label name, int address, char data, char relocatable, int DataLength); // declare on a node for new label
char nameExist(LabelNode **head, char * name);
LabelNode *mallocLabelNode( label name, int address, char data, char relocatable, int DataLength);  // sub function that doing the allocation process of the node
int getAddress(LabelNode **head, label name);   // return the address of the name you gave the function
int makeDataCode(LabelNode **head, int IC);  //make all the data node to code Node by changing the address to IC+100 and the boolean value
void freeLabelTableList(LabelNode **head);  // get the head of the list and free all the list
void makeEntry(LabelNode **head, label name);


unsigned int hash(char *key);    // take the name of the command and return the place in the array
HashTable * createHashTable(void);    // create and table allocate memory to a table parameter and make inside table array of node
void insert(HashTable **table, char* name, Pips *sNumber);  // Create & insert the compiler saveName node to the Hash table by its name
void setup_Table(HashTable **table); // set up all the constant should by in the array
Pips *search(HashTable **table, char* name);   // the function search in table the Pip code by the given name
void free_table(HashTable **table);  // take the table and free all the Node in the array
void free_Node(HashNode **node);

//function deal with Pips


#endif
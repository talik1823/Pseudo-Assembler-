//
// Created by Tal on 13/08/2024.
//

#ifndef THEPROGECT_CONSTANTVAR_H
#define THEPROGECT_CONSTANTVAR_H

#define MOV 0b000000000000000
#define CMP 0b000100000000000
#define ADD 0b001000000000000
#define SUB 0b001100000000000
#define LEA 0b010000000000000
#define CLR 0b010100000000000
#define NOT 0b011000000000000
#define INC 0b011100000000000
#define DEC 0b100000000000000
#define JMP 0b100100000000000
#define BNE 0b101000000000000
#define RED 0b101100000000000
#define PRN 0b110000000000000
#define JSR 0b110100000000000
#define RTS 0b111000000000000
#define STOP 0b111100000000000

#define A 0b000000000000100
#define R 0b000000000000010
#define E 0b000000000000001

/* The Addressing methods  are zero, one ,two, three and we will distinguish Source
 * method and Destination method by S or D in the beginning of the macro  */

#define D_ZERO  0b000000000001000
#define D_ONE   0b000000000010000
#define D_TWO   0b000000000100000
#define D_THREE 0b000000001000000

#define S_ZERO  0b000000010000000
#define S_ONE   0b000000100000000
#define S_TWO   0b000001000000000
#define S_THREE 0b000010000000000



static Pips mov = {MOV};
static Pips cmp = { CMP};
static Pips add = {ADD};
static Pips sub = {SUB};
static Pips lea = {LEA};
static Pips clr = {CLR};
static Pips not = {NOT};
static Pips inc = {INC};
static Pips dec = {DEC};       /*  NOTE: the bit is using in the two's complement method so pay attention to that   * */
static Pips jmp = {JMP};
static Pips bne = {BNE};
static Pips red = {RED};
static Pips prn = {PRN};
static Pips jsr = {JSR};
static Pips rts = {RTS};
static Pips stop = {STOP};

static Pips a = {A};
static Pips r = {R};
static Pips e = {E};

static Pips d_zero = {D_ZERO};
static Pips d_one = {D_ONE};
static Pips d_two = {D_TWO};
static Pips d_three = {D_THREE};

static Pips s_zero = {S_ZERO};
static Pips s_one = {S_ONE};
static Pips s_two = {S_TWO};
static Pips s_three = {S_THREE};


#endif //THEPROGECT_CONSTANTVAR_H

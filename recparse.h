#ifndef RECPARSE_H
#define RECPARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 1000

// flags
enum types {
    NUL, // 0 null
    NUM, // 1 number
    OPR, // 2 operator
    PAR, // 3 paranthesis
    ERR, // 4 error code for sysntax checking
    INT, // 5 init
};

typedef struct Param Param;

// node that holds operator and number input
struct Param {
    int type;
    double value;
    Param* left;
    Param* right;
};



static const char operands[] = "+-/*";
static const int OPRLEN = 4;

Param* r_parse(char*);
Param* _parse(Param*, char*);
Param* pushOprNode(Param*, int);
void pushDNode(Param*, double);
void apply(Param*, void fn(Param*));
int isoperand(char);
void removespaces(char* s);
Param* newNode(void);
void printNode(Param*);
double getresult(Param*);
void cleanup(Param *);
void reset(Param*);
void syntaxcheck(Param*, char*);

#endif

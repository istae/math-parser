#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 1000
#define DIGITS 40

// flags
enum types {
    NUL, // 0 null
    NUM, // 1 number
    OPR, // 2 operator
    PAR, // 3 paranthesis
    ERR, // 4 error code for sysntax checking
    INT, // 5 init
    PCL // paranthesis close
};

// node that holds operator and number input
struct Param {
    int type;
    double value;
    struct Param* left;
    struct Param* right;
    struct Param* parent;
};

typedef struct Param Param;

static const char operands[] = "+-/*";

Param* parse(char*);
Param* pushOprNode(Param*, int);
Param* pushDNode(Param*, double);
void apply(Param*, void fn(Param*));
int isoperand(char);
void removespaces(char* s);
Param* newNode(void);
Param* getroot(Param*);
void printNode(Param*);
double getresult(Param*);
Param* attachTrees(Param*, Param*);
void cleanup(Param *);
void reset(Param*);
void syntaxcheck(Param*, char*);

#endif

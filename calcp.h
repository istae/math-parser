#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 1000
#define DIGITS 40

enum types {
    NUL,
    NUM,
    OPR
};

// node that holds operator and number input
struct Param {
    int type;
    int value;
    struct Param* left;
    struct Param* right;
    struct Param* parent;
};

typedef struct Param Param;

static const char operands[] = "+-/*";

Param* parse(char*);
Param* pushOprNode(Param**, int);
Param* pushDNode(Param**, int);
void apply(Param*, void fn(Param*));
int isoperand(char);
void removespace(char* s);
Param* newNode(void);
Param* getroot(Param*);
void printNode(Param*);
int getresult(Param*);
Param* attachTrees(Param*, Param*);
void cleanup(Param *);
void reset(Param**, int);

#endif

#include "calcp.h"
/*
    max user input is 40, this can be changed in header file


    mostly stolen from
    http://interactivepython.org/runestone/static/pythonds/Trees/ParseTree.html
    &
    Practice of Programming

    TODO: most of these functions should be made static
*/

/* main function that creates a parse tree from user input
each '(' character create a new tree and is pushed onto a tree pointer stack,
at the end of main loop, trees are attached together into one main tree,
empty '(' trees are discarded
initially user input is cleansed by removespaces()
*/
Param* parse(char* c)
{
    int top = -1;
    Param* pstack[DIGITS];
    // initialize stack with NULL pointers
    for(int i=0;i<DIGITS; i++)
        pstack[i] = NULL;

    Param* params = newNode();
    pstack[++top] = params;

    removespaces(c);

    while (*c) {

        if (*c == '(') {
            if (top == DIGITS - 1)
                return NULL;
            pstack[++top] = newNode();
        }

        else if (isdigit(*c))
        {
            char buf[DIGITS];
            int i=0;
            do {
                buf[i++] = *c;
            } while(isdigit(*++c));
            --c;
            buf[i] = '\0';
            pstack[top] = pushDNode(pstack[top], atoi(buf));
        }


        else if (isoperand(*c)) {
            pstack[top] = pushOprNode(pstack[top], *c);
        }

        ++c;
    }

    // for each tree on top of stack, attach to one before
    while (top > 0) {
         pstack[top-1] = attachTrees(pstack[top], pstack[top-1]);
         --top;
    }

    return pstack[top];
}

// generate result from tree
int getresult(Param* p)
{
    Param* left = p->left;
    Param* right = p->right;

    if (left && right) {
        char c = p->value;
        switch(c) {
            case '+':
                return getresult(left) + getresult(right);
            case '-':
                return getresult(left) - getresult(right);
            case '/':
                return getresult(left) / getresult(right);
            case '*':
                return getresult(left) * getresult(right);
        }
    }
    else
        return p->value;
}

// callback function for printing nodes
void printNode(Param* p)
{
    printf("Type: %d, Value %c\n", p->type, p->value);
}

// Postorder
void apply(Param* p, void fn(Param*))
{
    if (p == NUL)
        return;

    apply(p->left, fn);
    apply(p->right, fn);
    fn(p);
}


// digit node
Param* pushDNode(Param* p, int val)
{
    Param* new = newNode();
    new->type = NUM;
    new->value = val;
    new->parent = p;

    if (p->left == NULL)
        p->left = new;
    else
        p->right = new;

    return new;
}

// operand node
Param* pushOprNode(Param* p, int opr)
{
    // find parent with NUl type, if out of bound create new node
    Param* n;
    while (p != NULL && p->type != NUL) {
        n = p;
        p = p->parent;
    }
    // special case where new node is needed
    if (p == NULL) {
        Param* new = newNode();
        new->value = opr;
        new->type = OPR;
        new->left = n;
        n->parent = new;
        return new;
    }

    else {
        p->value = opr;
        p->type = OPR;
        return p;
    }
}

Param* newNode()
{
    Param* p = malloc(sizeof(Param));
    if (p == NULL)
        printf("MALLOC BAD");
    p->type = NUL;
    p->value = 0;
    p->parent = NULL;
    p->left = NULL;
    p->right = NULL;
    return p;
}

int isoperand(char c)
{
    int i;
    for (i=0; operands[i] != '\0' && operands[i] != c; i++)
        ;
    return i < 4;
}

void removespaces(char* s)
{
    char* d = s;
    do
    {
        while(isspace(*s))
        s++;
    } while(*d++ = *s++);
}

Param* getroot(Param* p)
{
    Param* n;
    do {
        n = p;
        p = p->parent;
    } while(p != NULL);

    return n;
}

Param* attachTrees(Param* child, Param* parent)
{
    child = getroot(child);
    parent = getroot(parent);

    // empty parent, dont attach to main tree, discard it;
    if (parent->type  == NUL) {
        free(parent);
        return child;
    }

    parent->right = child;
    child->parent = parent;

    return parent;
}

void cleanup(Param *p)
{
    free(p);
}

void reset(Param** p)
{
    apply(*(p), cleanup);
}

#include "calcp.h"
/*
    max user input is 40, this can be changed in header file

    mostly stolen from
    http://interactivepython.org/runestone/static/pythonds/Trees/ParseTree.html
    &
    Practice of Programming

    TODO: most of these functions should be made static
        make parse() recursive
*/

/* main function that creates a parse tree from user input
each '(' character create a new tree and is pushed onto a tree pointer stack,
at the end of main loop, trees are attached together into one main tree,
does 3 runs on *c, 1) for removing spaces, 2) for sysntax check, 3) for tree build
*/
Param* parse(char* c)
{
    // Param stack
    int ptop = -1;
    Param* pstack[DIGITS];

    // initial node
    Param* params = newNode();
    pstack[++ptop] = params;

    removespaces(c);

    // run through input once, return syntax error if any
    syntaxcheck(params, c);
    if (params->type == ERR) {
        return params;
    }

    // in all honesty, this loop is BEGGING to be recursive, if implemented it
    // would get rid of the tree attachment function
    while (*c) {

        if (*c == '(') {
            pstack[++ptop] = newNode();
        }

        else if (isdigit(*c))
        {
            char buf[DIGITS];
            int i=0;
            do {
                buf[i++] = *c++;
            } while(isdigit(*c) || *c == '.');
            --c;
            buf[i] = '\0';
            pstack[ptop] = pushDNode(pstack[ptop], atof(buf));
        }


        else if (isoperand(*c))
            pstack[ptop] = pushOprNode(pstack[ptop], *c);

        ++c;
    }

    // for each tree on ptop of stack, attach to one before
    for (;ptop>0; ptop--)
        pstack[ptop-1] = attachTrees(pstack[ptop], pstack[ptop-1]);

    return pstack[ptop];
}

// generate result from tree
double getresult(Param* p)
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
    if (p == NULL)
        return;

    apply(p->left, fn);
    apply(p->right, fn);
    fn(p);
}


// digit node
Param* pushDNode(Param* p, double val)
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
    return i < OPRLEN;
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
    while (p != NULL) {
        n = p;
        p = p->parent;
    }
    return n;
}

// child  = pstack[top], parent = pstack[top-1]
Param* attachTrees(Param* child, Param* parent)
{
    child = getroot(child);
    parent = getroot(parent);

    // empty parent ex: (3) + 2
    if (parent->type  == NUL) {
        free(parent);
        return child;
    }

    // ex: 3 + (2)
    if (child->type == NUL && child->left->type == NUM) {
        parent->right = child->left;
        child->left->parent = parent;
        free(child);
        return parent;
    }

    // normal cases
    parent->right = child;
    child->parent = parent;

    return parent;
}

void syntaxcheck(Param* p, char* c)
{
    char* o = c;    //store original address
    int state = INT;
    int branch = 0; // '(' and ')' count, must be zero at the end
    int decimal;

    while (*c) {

        if (*c == '(') {
            if (state == NUM)
                break;
            state = PAR;
            branch++;
        }

        else if (*c == ')') {
            if (branch == 0 || state == PAR)
                break;
            branch--;
        }

        // get digit and '.', break if more than one '.'
        else if (isdigit(*c)) {
            state = NUM;
            decimal=0;
            do {
                ++c;
                if (*c == '.')
                    decimal++;
            } while (isdigit(*c) || *c == '.');
            --c;
            if (decimal > 1 || *c == '.')
                break;
        }

        else if (isoperand(*c)) {
            if (state == OPR || state == INT)
                break;
            state = OPR;
        }

        else
            break;

        ++c;
    }

    if (*c != '\0' || branch != 0 || state == OPR) {
        p->value = (c - o) * sizeof(char); // how far c has walked
        p->type = ERR;
        return;
    }
}

void cleanup(Param *p)
{
    free(p);
}

void reset(Param* p)
{
    apply(p, cleanup);
}

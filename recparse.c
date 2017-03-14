#include "recparse.h"
/*
    Esad Akar
    eakar2012@fau.edu

    Original: March 2017
    Last Edit: March 2017

    recparse is recursive parse

    recursive version of parse()
    this method removes the parent attribute from Param
*/

Param* r_parse(char* c)
{
    // initial node
    Param* root = newNode();

    removespaces(c);
    syntaxcheck(root, c);
    if (root->type == ERR) {
        return root;
    }

    return _parse(root, c);
}

Param* _parse(Param* root, char* c)
{
    while (*c) {

        if (*c == '(') {

            Param* child = _parse(newNode(), ++c); // recursive

            // ex: ((3+3))
            if (root->type == NUL) {
                Param* del = root;
                root = child;
                free(del);
            }
            // ex: 3 + (3)
            else if (child->type == NUL && child->left->type == NUM) {
                root->right = child->left;
                free(child);
            }
            else {
                root->right = child;
            }
            break;
        }

        else if (isdigit(*c))
        {
            char buf[SIZE];
            int i=0;
            do {
                buf[i++] = *c++;
            } while(isdigit(*c) || *c == '.');
            --c;
            buf[i] = '\0';
            pushDNode(root, atof(buf));
        }

        else if (isoperand(*c))
            root = pushOprNode(root, *c);

        ++c;
    }

    return root;
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
void pushDNode(Param* p, double val)
{
    Param* new = newNode();
    new->type = NUM;
    new->value = val;

    if (p->left == NULL)
        p->left = new;
    else
        p->right = new;
}

// operand node
Param* pushOprNode(Param* p, int opr)
{
    if (p->type != NUL) {
        Param* new = newNode();
        new->value = opr;
        new->type = OPR;
        new->left = p;
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

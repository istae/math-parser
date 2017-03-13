#include "recparse.h"
/*
    recursive version of parse()
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
            child = getroot(child);

            if (root->type == NUL) {
                Param* del = root;
                root = child;
                free(del);
            }
            else if (child->type == NUL && child->left->type == NUM) {
                root->right = child->left;
                child->left->parent = root;
                free(child);
            }
            else {
                root->right = child;
                child->parent = root;
            }
            break;
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
            root = pushDNode(root, atof(buf));
        }

        else if (isoperand(*c))
            root = pushOprNode(root, *c);

        ++c;
    }

    return getroot(root);
}

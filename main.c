#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "recparse.h"

int main(int argc, char** argv)
{
    char buffer[SIZE]; //SIZE define in header
    int len;

    while (fgets(buffer, SIZE, stdin) != NULL) {
        Param* tree = r_parse(buffer);
        if (tree->type != ERR) {
            double result = getresult(tree);
            printf("= %.3lf\n", result);
        }
        else {
            printf("\nsyntax error!\n%s\n", buffer);
            printf("%*c\n", (int)tree->value, 94);
        }
        reset(tree);
    }

    return 0;
}

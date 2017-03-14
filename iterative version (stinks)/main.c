#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "calcp.h"

int main(int argc, char** argv)
{
    char buffer[SIZE];
    int len;

    while (fgets(buffer, SIZE, stdin) != NULL) {
        Param* tree = parse(buffer);
        if (tree->type != ERR) {
            double result = getresult(getroot(tree));
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

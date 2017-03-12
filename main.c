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
        if (tree == NULL)
            printf("\nERROR: systax error or input too long\n");
        else if (tree->type != ERR) {
            int result = getresult(getroot(tree));
            reset(tree);
            printf("---------------------: %d\n", result);
        }
        else {
            printf("\nsyntax error!\n%s\n", buffer);
            printf("%*c\n", tree->value, 94);
            reset(tree);
        }
    }

    return 0;
}

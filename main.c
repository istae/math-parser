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
        else {
            int result = getresult(getroot(tree));
            reset(&tree, DIGITS);
            printf("---------------------: %d\n", result);
        }
    }

    return 0;
}

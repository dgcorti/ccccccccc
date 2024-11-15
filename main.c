#include <stdio.h>
#include "scanner.h"

int main(void) 
{
    TOKEN t;
    while ((t = Scanner()) != 13) 
    {
        char tokenResultante[17];
        formarToken(t, tokenResultante);
        printf("%d,%s->%s\n", t, tokenResultante, Buffer());  
    }
}

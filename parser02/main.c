#include <stdio.h>
#include "tinyexpr.h"

int main(void) {
    printf("%.2f\n", te_interp("5! + 6!", 0)); //retorno 840
    return 0;
}
#include <stdio.h>
#include "data-structure/sds.h"

sds testCreateSds(char *s) {
    return sdsnew(s);
}

int main() {
    sds str = testCreateSds("abcdefghijklasdfaasdfasdfasdfasdfasdfasdfasdfasdfasdfa");
    printf("sds:%s\n", str);
    return 0;
}


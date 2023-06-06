#include <stdio.h>
#include<string.h>
#include "data-structure/sds.h"

void testSds() {
    printf("sds8 length:%lu\n", sizeof(struct sdshdr8));
    printf("sds16 length:%lu\n", sizeof(struct sdshdr16));
    printf("sds32 length:%lu\n", sizeof(struct sdshdr32));
    printf("sds64 length:%lu\n", sizeof(struct sdshdr64));
}



int main() {
    testSds();
    return 0;
}


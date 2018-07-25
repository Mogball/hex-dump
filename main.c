#include <stdlib.h>
#include <stdio.h>

#define REPLACE_CALLS
#include "memory_file.h"

int main(void) {
    FILE *file = fopen("mock.txt", "w");
    static unsigned char buf[1 << 16];
    int i = 0;
    int j = 0;
    for (; i < 4; ++i) {
        for (j = 0; j < (1 << 16); ++j) {
            buf[j] = (unsigned char) (rand() % 256);
        }
        fwrite(buf, 1, 1 << 18, file);
    }
    heavy_dump(file, stdout);
    return 0;
}

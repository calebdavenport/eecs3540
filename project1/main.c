#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SHA512.h"

long fsize(const char *filename);
long messageBlocks(off_t message);

int main() {
    char eof = 0;
	FILE *fp;
    char block[1024];
    char buffer[128];
    unsigned long long currentHash[8];

    for (int i = 0; i < 8; i++) {
        currentHash[i] = initialHash[i];
        printf("%llx\n", currentHash[i]);
    }

	fp = fopen("test.txt", "rb");
	printf("TEST\n");
    printf("0x%lx\n", fsize("test.txt"));
    printf("0x%lx\n", messageBlocks(111));
    if ( fp != NULL ) {
        int read = 0;
        while((read = fread(buffer, 1, 128, fp)) > 0) {
            for(int i = 0; i < read; i++) {
                if (i > 111) {
                    ; // If the message ends above this range, another
                      // block must be made
                }
                printf("%c", buffer[i]);
            }
            printf("\n");
            printf("%x", *buffer);
            printf("\n---------\n");
        }
    }
    fclose(fp);

}


// Return the size of the file (in bytes)
long fsize(const char *filename) {
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

// Return the number of blocks the message requires
// Arguments:
//     message: length of message (in bytes)
long messageBlocks(off_t message) {
    off_t totalSize = (8 * message) + 128 + 1;
    return (totalSize / 1024) + 1;
}

long long rightRot(long long x, unsigned int d) {
    return (x >> d) | (x << (64 - d));
}

long long ch_function(long long x, long long y, long long z) {
    return (x & y) ^ (~x & z);
}

long long maj_function(long long x, long long y, long long z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

long long sigma_0(long long x) {
    return rightRot(x, 28) ^ rightRot(x, 34) ^ rightRot(x, 39);
}

long long sigma_1(long long x) {
    return rightRot(x, 14) ^ rightRot(x, 18) ^ rightRot(x, 41);
}

long long delta_0(long long x) {
    return rightRot(x, 1) ^ rightRot(x, 8) ^ (x >> 7);
}

long long delta_1(long long x) {
    return rightRot(x, 19) ^ rightRot(x, 61) ^ (x >> 6);
}

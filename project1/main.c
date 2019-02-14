#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SHA512.h"

void compressionFunction(unsigned long long block[16]);
long fsize(const char *filename);
long messageBlocks(off_t message);
unsigned long long rightRot(unsigned long long x, unsigned int d);
unsigned long long ch_function(unsigned long long x,
                               unsigned long long y,
                               unsigned long long z);
unsigned long long maj_function(unsigned long long x,
                                unsigned long long y,
                                unsigned long long z);
unsigned long long sigma_0(unsigned long long x);
unsigned long long sigma_1(unsigned long long x);
unsigned long long delta_0(unsigned long long x);
unsigned long long delta_1(unsigned long long x);

static unsigned long long currentHash[8];

int main() {
    char eof = 0;
	FILE *fp;
    static unsigned long long block[16];

    for (int i = 0; i < 8; i++) {
        currentHash[i] = initialHash[i];
    }

	fp = fopen("test.txt", "rb");
    if ( fp != NULL ) {
        int read = 0;
        char subblock[128];
        while((read = fread(subblock, 1, 128, fp)) > 0) {
            printf("%x\n", read);
            for(int i = 0; i < read; i++) {
                block[i / 8] = block[i / 8] >> 8 * (7 - (i % 8));
                block[i / 8] += subblock[i];
                block[i / 8] = block[i / 8] << 8 * (7 - (i % 8));
                if (i > 111) {
                    ; // If the message ends above this range, another
                      // block must be made
                }
                printf("%016llx\n", block[i / 8]);
            }
            // printf("%llx", 3);
            compressionFunction(block);
            //printf("%llx", compressionFunction(block));
            //printf("~~~~\n");
            break; //TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
            //printf("%d", block);
            // printf("\n---------\n");
        }
    }
    fclose(fp);

}

void compressionFunction(unsigned long long block[16]) {
    unsigned long long w[80];
    unsigned long long t1;
    unsigned long long t2;
    unsigned long long a = currentHash[0];
    unsigned long long b = currentHash[1];
    unsigned long long c = currentHash[2];
    unsigned long long d = currentHash[3];
    unsigned long long e = currentHash[4];
    unsigned long long f = currentHash[5];
    unsigned long long g = currentHash[6];
    unsigned long long h = currentHash[7];
    for (int i = 0; i < 16; i++) {
        w[i] = block[i];
    }
    for (int j = 0; j < 80; j++) {
        if (j >= 16) {
            w[j] = delta_1(w[j-2]) + w[j-7] + delta_0(w[j-15]) + w[j-16];
            // printf("%llx\n", w[j]);
        }
        t1 = h + sigma_1(e) + ch_function(e, f, g) + constants[j] + w[j];
        t2 = sigma_0(a) + maj_function(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    //printf("__%llx\n", w[79]);
    currentHash[0] += a;
    currentHash[1] += b;
    currentHash[2] += c;
    currentHash[3] += d;
    currentHash[4] += e;
    currentHash[5] += f;
    currentHash[6] += g;
    currentHash[7] += h;
    //printf("_-_-%llx", currentHash);

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

unsigned long long rightRot(unsigned long long x, unsigned int d) {
    return (x >> d) | (x << (64 - d));
}

unsigned long long ch_function(unsigned long long x,
                               unsigned long long y,
                               unsigned long long z) {
    return (x & y) ^ (~x & z);
}

unsigned long long maj_function(unsigned long long x,
                                unsigned long long y,
                                unsigned long long z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

unsigned long long sigma_0(unsigned long long x) {
    return rightRot(x, 28) ^ rightRot(x, 34) ^ rightRot(x, 39);
}

unsigned long long sigma_1(unsigned long long x) {
    return rightRot(x, 14) ^ rightRot(x, 18) ^ rightRot(x, 41);
}

unsigned long long delta_0(unsigned long long x) {
    return rightRot(x, 1) ^ rightRot(x, 8) ^ (x >> 7);
}

unsigned long long delta_1(unsigned long long x) {
    return rightRot(x, 19) ^ rightRot(x, 61) ^ (x >> 6);
}

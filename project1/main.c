#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SHA512.h"


void addSubblock(unsigned char subblock[], unsigned long long block[], int i) {
    block[i / 8] = block[i / 8] >> 8 * (7 - (i % 8));
    block[i / 8] += subblock[i];
    block[i / 8] = block[i / 8] << 8 * (7 - (i % 8));
}

void compressionFunction(unsigned long long block[], unsigned long long currentHash[]) {
    unsigned long long w[80], t1, t2;
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
    currentHash[0] += a;
    currentHash[1] += b;
    currentHash[2] += c;
    currentHash[3] += d;
    currentHash[4] += e;
    currentHash[5] += f;
    currentHash[6] += g;
    currentHash[7] += h;
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
    off_t totalSize = message + 16 + 1;
    return (totalSize + 127) / 128;
}

int main() {
    FILE *fp;
    static unsigned long long block[16], currentHash[8];

    for (int i = 0; i < 8; i++) {
        currentHash[i] = initialHash[i];
    }

    fp = fopen("test4.txt", "rb");
    if ( fp != NULL ) {
        int read = 0, M = 0, currentM = 0, bit_used = 0;
        unsigned char subblock[128];
        M = messageBlocks(fsize("test4.txt"));
        for (int currentM = 1; currentM <= M; currentM++) {
            read = fread(subblock, 1, 128, fp);
            memset(block, 0, sizeof(block));
            if (read == 0x80) {
                for(int i = 0; i < 128; i++) {
                    addSubblock(subblock, block, i);
                }
                compressionFunction(block, currentHash);
                continue;
            }
            for(int i = 0; i < read; i++) {
                addSubblock(subblock, block, i);
            }
            if (!bit_used) {
                bit_used = 1;
                subblock[read] = 0x80;
                addSubblock(subblock, block, read);
            }
            if (currentM == M) {
                block[15] = (8 * fsize("test4.txt"));
            }
            compressionFunction(block, currentHash);
        }
    }
    fclose(fp);
    for (int i = 0; i < 8; i++) {
        printf("%llx", currentHash[i]);
    }
}


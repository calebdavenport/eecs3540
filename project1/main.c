#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SHA512.h"

long fsize(const char *filename);
long messageBlocks(off_t message);

int main() {
	FILE *fp;
    char block[1024];

    memset(block, 'A', sizeof block);

	fp = fopen("test.txt", "r");
	// fprintf(fp, "This is testing for fprintf...\n");
	// fputs("This is testing for fputs...\n", fp);
	fclose(fp);
	printf("TEST\n");
    printf("0x%lx\n", fsize("test.txt"));
    printf("0x%lx\n", messageBlocks(128));

/*
	for (int i = 0; i < 128; ++i)
	{
		block[i] = 'A';
	}
*/
    for (int i = 0; i < 8; i++) {
        // printf("%c", block[i]);
        for (int j = 0; j < 128; j++) {
            int index = 128 * i + j;
            printf("%c", block[index]);
        }
        printf("\n");
    }
	return 0;

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


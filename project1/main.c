#include <stdio.h>
#include <string.h>
#include "SHA256.h"

int main() {
	FILE *fp;
    char block[128];

    memset(block, 'A', sizeof block);

	fp = fopen("test.txt", "r");
	// fprintf(fp, "This is testing for fprintf...\n");
	// fputs("This is testing for fputs...\n", fp);
	fclose(fp);
	printf("TEST\n");

/*
	for (int i = 0; i < 128; ++i)
	{
		block[i] = 'A';
	}
*/
    for (int i = 0; i < 16; i++) {
        printf("%c", block[i]);
        for (int j = 0; j < 8; j++) {
            int index = 8 * i + j;
            printf("%c", block[index]);
        }
        printf("\n");
    }
	return 0;
}

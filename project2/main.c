#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 4096

unsigned char file_system[BLOCK_SIZE*NUM_BLOCKS];

void init_FAT() {
    unsigned short f1 = (file_system[0] << 8) | file_system[1];
    f1 = (unsigned short)(0x8 << 12) | 0x000;
    file_system[0] = (f1 >> 8);
    file_system[1] = (f1 & 0x00FF);
    unsigned short f2 = (file_system[2] << 8) | file_system[3];
    f2 = (unsigned short)(0x8 << 12) | 0x001;
    file_system[2] = (f2 >> 8);
    file_system[3] = (f2 & 0x00FF);
    unsigned short f3 = (file_system[4] << 8) | file_system[5];
    f3 = (unsigned short)(0x4 << 12) | 0x002;
    file_system[4] = (f3 >> 8);
    file_system[5] = (f3 & 0x00FF);
}

void map() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        switch (file_system[i * 2] & 0xF0) {
            case 0x80:
                printf("F");
                break;
            case 0x40:
                printf("D");
                break;
            case 0x20:
                printf("*");
                break;
            default:
                printf("_");
                break;
        }
        if (i % 128 == 127) {
            printf("\n");
        }
    }
    /*
    printf("%x\n", file_system[0] & 0xF0);
    printf("%x\n", file_system[1]);
    printf("%x\n", file_system[2]);
    printf("%x\n", file_system[3]);
    printf("%x\n", file_system[4]);
    printf("%x\n", file_system[5]);
    */
}

int main() {
    init_FAT();
    map();
    return 0;
}


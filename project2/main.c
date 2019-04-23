#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 4096

unsigned char file_system[BLOCK_SIZE*NUM_BLOCKS];

void init_FAT() {
    unsigned short f1 = (file_system[0] << 8) | file_system[1];
    f1 = (unsigned short)(0x8 << 12) | 0x001;
    file_system[0] = (f1 >> 8);
    file_system[1] = (f1 & 0x00FF);
    unsigned short f2 = (file_system[2] << 8) | file_system[3];
    f2 = (unsigned short)(0x8 << 12) | 0x1FFF;
    file_system[2] = (f2 >> 8);
    file_system[3] = (f2 & 0x00FF);
    unsigned short f3 = (file_system[4] << 8) | file_system[5];
    f3 = (unsigned short)(0x4 << 12) | 0x1FFF;
    file_system[4] = (f3 >> 8);
    file_system[5] = (f3 & 0x00FF);
    for (int i = 6; i < NUM_BLOCKS; i++) {
        file_system[i] = 0x00;
    }
}

void map() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        switch (file_system[i * 2] & 0xE0) {
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
        if (i % 64 == 63) {
            printf("\n");
        }
    }
}

int main() {
    while(1) {
        char c;
        scanf(" %c", &c);
        switch (c) {
            case 'm':
                map();
                break;
            case 'i':
                init_FAT();
                break;
            case 'q':
                return 0;
                break;
            default:
                printf("Unknown Command");
        }
    }
    return 0;
}


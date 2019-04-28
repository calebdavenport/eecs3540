#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

void output_fs_to_file() {
    FILE *f = fopen("filesystem", "wb");
    fwrite(file_system, sizeof(unsigned char), sizeof(file_system), f);
    fclose(f);
}

void input_file_to_fs() {
    if (access("filesystem", F_OK) == -1) {
        printf("filesystem file not found. Creating...\n");
        init_FAT();
        output_fs_to_file();
    } else {
        FILE *f = fopen("filesystem", "rb");
        fread(file_system, sizeof(unsigned char), sizeof(file_system), f);
        fclose(f);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No command entered.\n");
        return 2;
    }
    input_file_to_fs();
    if (strcmp(argv[1], "map") == 0) {
        printf("Outputting Map");
        map();
    } else if (strcmp(argv[1], "format") == 0) {
        printf("Formatting file system...");
        init_FAT();
        printf("done\n");
    } else {
        printf("Unknown Command\n");
    }
    output_fs_to_file();
    return 0;
}


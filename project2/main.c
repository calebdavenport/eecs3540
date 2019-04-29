#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 4096

char file_system[BLOCK_SIZE*NUM_BLOCKS];

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

char check_empty_file(int i) {
    for (int j = 0; j < 48; j++) {
        if (file_system[BLOCK_SIZE * 2 + i + j] != 0) {
            return 0;
        }
    }
    return 1;
}

void add_file(char *filename, char *file_contents) {
    int i;
    for (i = 0; i < BLOCK_SIZE; i = i + 48) {
        if (check_empty_file(i)) {
            break;
        }
    }
    strcpy(file_system + BLOCK_SIZE * 2 + i, filename);

    short start_block;
    for (int j = 0; i < NUM_BLOCKS; j++) {
        if ((file_system[j * 2] & 0xE0) == 0x00) {
            start_block = j;
            file_system[j * 2] = 0x3F;
            file_system[j * 2 + 1] = 0xFF;
            break;
        }
    }
    file_system[BLOCK_SIZE * 2 + i + 32] = start_block >> 8;
    file_system[BLOCK_SIZE * 2 + i + 33] = start_block & 0xFF;

    strcpy(file_system + BLOCK_SIZE * start_block, file_contents);
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
        printf("Outputting Map...\n");
        map();
    } else if (strcmp(argv[1], "format") == 0) {
        printf("Formatting file system...");
        init_FAT();
        printf("done\n");
    } else if (strcmp(argv[1], "write") == 0) {
        if (argc < 4) {
            printf("Not enough arguments.\n");
            return 4;
        }
        printf("Adding file...");
        add_file(argv[2], argv[3]);
        printf("done\n");
    } else {
        printf("Unknown Command\n");
    }
    output_fs_to_file();
    return 0;
}


#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define OPTIONS "hi:o:"

// Credits to Eugene Chou for these 3 helper functions and statbuf struct

struct stat statbuf;

// Returns lower nibble of a uin8
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

// Returns upper
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

typedef enum graph_types { h, v, u, i, o } graph_types;
// Structure inspired by the asgn3.pdf
int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    //bool undirected = false;
    FILE *file_pointer_in = stdin;
    FILE *file_pointer_out = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        // Flag each program depending on what the program argument was
        case 'h': help = true; break;
        //case 'v': verbose = true; break;
        //case 'u': undirected = true; break;
        case 'i':
            if ((file_pointer_in = fopen(optarg, "r")) == NULL) {
                printf("error opening files");
                fclose(file_pointer_out);
                exit(1);
            }
            break;
        case 'o':
            if ((file_pointer_out = fopen(optarg, "w")) == NULL) {
                printf("error opening filesdf");
                fclose(file_pointer_in);
                exit(1);
            }
            break;
        default: help = true;
        }
    }

    if (help == true) {
        fprintf(file_pointer_out,
            "SYNOPSIS\n  A Hamming(8, 4) systematic code generator.\n\nUSAGE\n  ./encode [-h] [-i "
            "infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -i "
            "infile      Input data to encode.\n  -o outfile     Output of encoded data.\n");
        fclose(file_pointer_in);
        fclose(file_pointer_out);
        return 0;
    }

    // Make G
    BitMatrix *G = bm_create(4, 8);
    for (uint32_t i = 0; i < 4; i++) {
        bm_set_bit(G, i, i);
    }
    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            } else {
                bm_set_bit(G, i, j + 4);
            }
        }
    }
    int byte = 0;
    char code1 = 0;
    char code2 = 0;
    while ((byte = fgetc(file_pointer_in)) != EOF) {
        uint8_t msg1 = lower_nibble(byte);
        uint8_t msg2 = upper_nibble(byte);
        code1 = ham_encode(G, msg1);
        code2 = ham_encode(G, msg2);
        fputc(code1, file_pointer_out);
        fputc(code2, file_pointer_out);
    }

    //clean up data and set file perms
    // Credits to Eugene Chou for the fstat and fchmod file perm example
    fstat(fileno(file_pointer_in), &statbuf);
    fchmod(fileno(file_pointer_out), statbuf.st_mode);
    bm_delete(&G);
    fclose(file_pointer_in);
    fclose(file_pointer_out);
}

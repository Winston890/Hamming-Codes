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
#define OPTIONS "hvi:o:"

// Credits to Eugene Chou for these 3 helper functions and statbuf struct

struct stat statbuf;

// 2 nibbles = a bite
uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

typedef enum graph_types { h, v, u, i, o } graph_types;
// Structure inspired by the asgn3.pdf
int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    bool verbose = false;
    //bool undirected = false;
    FILE *file_pointer_in = stdin;
    FILE *file_pointer_out = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        // Flag each program depending on what the program argument was
        case 'h': help = true; break;
        case 'v': verbose = true; break;
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
            "SYNOPSIS\n  A Hamming(8, 4) systematic code decoder.\n\nUSAGE\n  ./decode [-h] [-v] "
            "[-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v   "
            "          Print statistics of decoding to stderr.\n  -i infile      Input data to "
            "decode.\n  -o outfile     Output of decoded data.\n");
        fclose(file_pointer_in);
        fclose(file_pointer_out);
        return 0;
    }
    // Make H transpose
    BitMatrix *H = bm_create(8, 4);
    for (uint32_t i = 0; i < 4; i++) {
        bm_set_bit(H, i + 4, i);
    }
    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            } else {
                bm_set_bit(H, i, j);
            }
        }
    }

    int code1 = 0;
    int code2 = 0;
    uint8_t msg1 = 0;
    uint8_t msg2 = 0;
    double total_bytes = 0;
    double uncorrected_errors = 0;
    int corrected_errors = 0;
    while ((code1 = fgetc(file_pointer_in)) != EOF) {
        code2 = fgetc(file_pointer_in);
        HAM_STATUS byte1 = ham_decode(H, code1, &msg1);
        HAM_STATUS byte2 = ham_decode(H, code2, &msg2);
        char full_byte = pack_byte(msg2, msg1);
        fputc(full_byte, file_pointer_out);
        total_bytes += 2;
        if (byte1 == HAM_ERR) {
            uncorrected_errors += 1;
        }
        if (byte2 == HAM_ERR) {
            uncorrected_errors += 1;
        }
        if (byte1 == HAM_CORRECT) {
            corrected_errors += 1;
        }
        if (byte2 == HAM_CORRECT) {
            corrected_errors += 1;
        }
    }

    if (verbose) {
        double error_rate = uncorrected_errors / total_bytes;
        fprintf(stderr,
            "\nTotal bytes processed: %.0f\nUncorrected errors: %.0f\nCorrected errors: %d\nError "
            "rate: %f\n",
            total_bytes, uncorrected_errors, corrected_errors, error_rate);
    }
    // Credits to Eugene Chou for the fstat and fchmod example
    fstat(fileno(file_pointer_in), &statbuf);
    fchmod(fileno(file_pointer_out), statbuf.st_mode);
    bm_delete(&H);
    fclose(file_pointer_in);
    fclose(file_pointer_out);
}

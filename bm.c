#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));
    assert(m);
    m->vector = bv_create(rows * cols);
    m->rows = rows;
    m->cols = cols;
    return m;
}

void bm_delete(BitMatrix **m) {
    bv_delete(&(*m)->vector);
    free(*m);
    *m = NULL;
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = r * m->cols + c;
    bv_set_bit(m->vector, i);
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = r * m->cols + c;
    bv_clr_bit(m->vector, i);
}

//Developed together with Miles
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = r * m->cols + c;
    return bv_get_bit(m->vector, i);
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length);
    for (uint32_t i = 0; i < length; i++) {
        if (byte & (1 << i)) {
            bm_set_bit(m, 0, i);
        }
    }
    return m;
}

uint8_t bm_to_data(BitMatrix *m) {
    uint8_t byte = 0;
    for (uint32_t i = 0; i < 8; i++) {
        if (bm_get_bit(m, 0, i) == 1) {
            byte |= 1 << i;
        }
    }
    return byte;
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *product = bm_create(A->rows, B->cols);
    for (uint32_t k = 0; k < bm_cols(B); k++) {
        int column_sum = 0;
        for (uint32_t j = 0; j < bm_rows(B); j++) {
            column_sum += bm_get_bit(A, 0, j) & bm_get_bit(B, j, k);
        }
        column_sum %= 2;
        if (column_sum == 1) {
            bm_set_bit(product, 0, k);
        }
    }
    return product;
}

void bm_print(BitMatrix *m) {
    printf("Matrix: \n");
    for (uint32_t i = 0; i < m->rows; i++) {
        for (uint32_t j = 0; j < m->cols; j++) {
            if (bm_get_bit(m, i, j)) {
                printf("1");
            } else {
                printf("0");
            }
        }
        printf("\n");
    }
}

#include "bv.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) calloc(1, sizeof(BitVector));
    v->length = length;
    int bytes = 0;
    // Calculate bytes needed
    if (length == 0) {
        bytes = 0;
    } else if (length % 8 == 0) {
        bytes = length / 8;
    } else {
        bytes = length / 8 + 1;
    }
    v->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    if (v == NULL) {
        free(v);
        return NULL;
    }
    assert(v);
    return v;
}

void bv_delete(BitVector **v) {
    free((*v)->vector);
    free(*v);
    *v = NULL;
}

uint32_t bv_length(BitVector *v) {
    return v->length;
}

void bv_set_bit(BitVector *v, uint32_t i) {
    int offset = i % 8;
    v->vector[i / 8] |= 1 << offset;
}

void bv_clr_bit(BitVector *v, uint32_t i) {
    int offset = i % 8;
    v->vector[i / 8] = v->vector[i / 8] & ~(1 << offset);
}

// Shift left, and bit with 1, shift left sem around
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    return (v->vector[i / 8] >> (i % 8)) & 1;
}

void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    int offset = i % 8;
    v->vector[i / 8] = v->vector[i / 8] ^ (bit << offset);
}

void bv_print(BitVector *v) {
    printf("Numerical value: %u\n", *v->vector);
    printf("In array notation: ");
    uint8_t bit = 0;
    for (uint32_t i = 0; i < v->length; i++) {
        bit = bv_get_bit(v, i);
        printf("%u", bit);
    }
    printf("\n");
}

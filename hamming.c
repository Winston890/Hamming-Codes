#include "hamming.h"

#include "bm.h"
#include "bv.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static int lookup[16] = { [0] = HAM_OK,
    [1] = 4,
    [2] = 5,
    [3] = HAM_ERR,
    [4] = 6,
    [5] = HAM_ERR,
    [6] = HAM_ERR,
    [7] = 3,
    [8] = 7,
    [9] = HAM_ERR,
    [10] = HAM_ERR,
    [11] = 2,
    [12] = HAM_ERR,
    [13] = 1,
    [14] = 0,
    [15] = HAM_ERR };

uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    //msg is a byte (with only a nibble of real info)
    BitMatrix *msg_matrix = bm_from_data(msg, 4);
    BitMatrix *code = bm_multiply(msg_matrix, G);
    bm_delete(&msg_matrix);
    uint8_t data = bm_to_data(code);
    bm_delete(&code);
    return data;
}

HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    BitMatrix *c = bm_from_data(code, 8);
    BitMatrix *error_syndrome = bm_multiply(c, Ht);
    int result = lookup[bm_to_data(error_syndrome)];
    if (result == HAM_OK) {
        *msg = code & 0x0F;
        bm_delete(&c);
        bm_delete(&error_syndrome);
        return HAM_OK;
    } else if (result == HAM_ERR) {
        bm_delete(&c);
        bm_delete(&error_syndrome);
        return HAM_ERR;
    } else {
        if (bm_get_bit(c, 0, result) == 1) {
            bm_clr_bit(c, 0, result);
        } else {
            bm_set_bit(c, 0, result);
        }
        *msg = bm_to_data(c) & 0x0F;
        bm_delete(&c);
        bm_delete(&error_syndrome);
        return HAM_CORRECT;
    }
}

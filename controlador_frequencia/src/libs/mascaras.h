#ifndef MASCARAS_H
#define MASCARAS_H


#define set_bit(x, bit_x) (x |= (1<<bit_x))

#define clr_bit(x, bit_x) (x &= ~(1<<bit_x))

#define tst_bit(x, bit_x) (x & (1<<bit_x))

#define cpl_bit(x, bit_x) (x ^= (1<<bit_x))


#endif
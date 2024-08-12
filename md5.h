#ifndef md5_H
#define md5_H

#include <stdint.h>// for uint32_t

// Initialization functions
void initMd5Vals(uint32_t *vars);
uint64_t countrounds(const char *input);
void md5(const char *initial_msg);
void setZero(uint32_t m[]);

void tolitendien(uint32_t *arr);
void to_bytes(uint32_t val, uint8_t *bytes);
void split_to_bytes(uint32_t *h, uint8_t *bytes);
void printSchedule(uint32_t *m);

// Logging functions
void printhash(uint32_t vars[]);
void bin(unsigned n);
void printinputhex(uint32_t input[]);
void printByteHash(uint8_t *h);

#endif
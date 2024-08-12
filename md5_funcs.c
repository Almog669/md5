#include <stdint.h> // for uint32_t
#include <stdbool.h>// for bool
#include <stdio.h>  // for printf
#include <string.h> // for strlen
#include <stdlib.h> // for calloc
#include "md5.h"

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

#define BIG_TO_LITTLE_ENDIAN(num) (\
    (((num) >> 24) & 0x000000FF) | \
    (((num) >> 8)  & 0x0000FF00) | \
    (((num) << 8)  & 0x00FF0000) | \
    (((num) << 24) & 0xFF000000))

// Function to initialize the array with MD5 initial values
void initMd5Vals(uint32_t *vars){
    vars[0] = 0x67452301;
    vars[1] = 0xefcdab89;
    vars[2] = 0x98badcfe;
    vars[3] = 0x10325476;
}

// Array of the 64 rotations needed in the MD5 algorithm
static const int r[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

// Precomputed K values for MD5
static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint64_t countrounds(const char *input){
    uint64_t len = strlen(input);
    uint64_t res;
    
    if(len < 56)
        res = 1;
    else if (len > 55 && len <= 64)
        res = 2;
    else if (len > 64 && (len % 64) < 56)
        res = len / 64 + 1;
    else if (len > 64 && (len % 64) > 55)
        res = len / 64 + 2;
    
    return res;
}

void printhash(uint32_t vars[]){
    printf("Hash Value : \n\n");
    for(int i = 0 ;i < 4; i++){
        printf("%08x", vars[i]);
    }
    printf("\n");
}

void printinputhex(uint32_t input[]){
    printf("Hash Value : \n\n");
    for(int i = 0 ;i < 16; i++){
        printf("%08x", input[i]);
    }
    printf("\n");
}

void bin(unsigned n){
    unsigned i = 1;
    for ( i = i << 31; i > 0; i = i / 2) {
        ((n & i) != 0)? printf("1"):printf("0");
    }
    printf("\n");
}

void tolitendien(uint32_t *arr){
    for (size_t i = 0; i < 16; i++){
        arr[i] = BIG_TO_LITTLE_ENDIAN(arr[i]);
    }
} 

// Function to compute MD5 hash for a given string
void md5(const char *initial_msg){
    printf("\nFirstly we initialize the packet 512 bits : \n\nAKA the messege schedule\n");
    printf("Rounds :%lu\n", countrounds(initial_msg));
    uint32_t h[4];
    uint8_t hash[16];
    size_t initial_len = strlen(initial_msg);
    int round = 1;

    // Initial hash values
    initMd5Vals(h);

    // Pre-processing: adding a single 1 bit
    size_t new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;
    uint8_t *msg = calloc(new_len + 64, 1);
    memcpy(msg, (uint8_t *)initial_msg, initial_len);
    msg[initial_len] = 128; // Add 1 bit, rest is 0

    // Append original length in bits at the end of the buffer
    uint64_t bits_len = 8 * initial_len;
    memcpy(msg + new_len, &bits_len, 8);

    // Process each 512-bit chunk
    for (int offset = 0; offset < new_len; offset += 64) {
        printf("Round : %d\n",round);round++;
        uint32_t *w = (uint32_t *)(msg + offset);
        uint32_t a, b, c, d, f, g, temp;
        printSchedule(w);
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        
        for (int i = 0; i < 64; i++) {
            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }
            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + K[i] + w[g]), r[i]);
            a = temp;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
    }
    //Convert the resulting hash to little-endian
    split_to_bytes(h, hash);
    printByteHash(hash);
    free(msg);
}

// Convert a 32-bit integer to little-endian byte order
void to_bytes(uint32_t val, uint8_t *bytes){
    bytes[0] = (uint8_t) (val);
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}

void split_to_bytes(uint32_t *h, uint8_t *bytes){
    int pos = 0;
    for (size_t i = 0; i < 4; i++){
        to_bytes(h[i], bytes + pos);
        pos += 4;
    }
}

void printSchedule(uint32_t *m){
    for (int i = 0; i < 16; i++){
        printf("m%d : ", i); bin(m[i]);
    }  
}

void printByteHash(uint8_t *h){
    printf("MD5 hash: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", h[i]);
    }
    printf("\n");
}
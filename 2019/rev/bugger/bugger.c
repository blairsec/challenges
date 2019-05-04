#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <link.h>
#define SHA512_DIGEST_LENGTH 64



#include <stddef.h>
typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;


struct pcg32_random_t* pcg32_random_t2;
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
#ifdef WORDS_BIGENDIAN
# define SWAP(n) (n)
#else
# define SWAP(n) \
    (((((n) << 56) | (((n) & 0x0000ff00LLU) << 40)) | \
      ((((n) & 0x00ff0000LLU) << 24) | (((n) & 0xff000000LLU) <<  8))) | \
     (((((n) >>  8) & 0xff000000LLU) | (((n) >> 24) & 0x00ff0000LLU)) | \
      ((((n) >> 40) & 0x0000ff00LLU) | ((n) >> 56))))
#endif


    #define NN 312
    #define MM 156
    #define MATRIX_A 0xB5026F5AA96619E9ULL
    #define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
    #define LM 0x7FFFFFFFULL /* Least significant 31 bits */


    /* The array for the state vector */
    static unsigned long long mt[NN]; 
    /* mti==NN+1 means mt[NN] is not initialized */
    static int mti=NN+1; 

    /* initializes mt[NN] with a seed */
    void init_genrand64(unsigned long long seed)
    {
        mt[0] = seed;
        for (mti=1; mti<NN; mti++) 
            mt[mti] =  (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
    }

    /* initialize by an array with array-length */
    /* init_key is the array for initializing keys */
    /* key_length is its length */
    void init_by_array64(unsigned long long init_key[],
             unsigned long long key_length)
    {
        unsigned long long i, j, k;
        init_genrand64(19650218ULL);
        i=1; j=0;
        k = (NN>key_length ? NN : key_length);
        for (; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 3935559000370003845ULL))
              + init_key[j] + j; /* non linear */
            i++; j++;
            if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
            if (j>=key_length) j=0;
        }
        for (k=NN-1; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 2862933555777941757ULL))
              - i; /* non linear */
            i++;
            if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
        }

        mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */ 
    }

    /* generates a random number on [0, 2^64-1]-interval */
    unsigned long long genrand64_int64(void)
    {
        int i;
        unsigned long long x;
        static unsigned long long mag01[2]={0ULL, MATRIX_A};

        if (mti >= NN) { /* generate NN words at one time */

            /* if init_genrand64() has not been called, */
            /* a default initial seed is used     */
            if (mti == NN+1) 
                init_genrand64(5489ULL); 

            for (i=0;i<NN-MM;i++) {
                x = (mt[i]&UM)|(mt[i+1]&LM);
                mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
            }
            for (;i<NN-1;i++) {
                x = (mt[i]&UM)|(mt[i+1]&LM);
                mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
            }
            x = (mt[NN-1]&UM)|(mt[0]&LM);
            mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

            mti = 0;
        }
      
        x = mt[mti++];

        x ^= (x >> 29) & 0x5555555555555555ULL;
        x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
        x ^= (x << 37) & 0xFFF7EEE000000000ULL;
        x ^= (x >> 43);

        return x;
    }

    /* generates a random number on [0, 2^63-1]-interval */
    long long genrand64_int63(void)
    {
        return (long long)(genrand64_int64() >> 1);
    }

    /* generates a random number on [0,1]-real-interval */
    double genrand64_real1(void)
    {
        return (genrand64_int64() >> 11) * (1.0/9007199254740991.0);
    }

    /* generates a random number on [0,1)-real-interval */
    double genrand64_real2(void)
    {
        return (genrand64_int64() >> 11) * (1.0/9007199254740992.0);
    }

    /* generates a random number on (0,1)-real-interval */
    double genrand64_real3(void)
    {
        return ((genrand64_int64() >> 12) + 0.5) * (1.0/4503599627370496.0);
    }




/* This array contains the bytes used to pad the buffer to the next
   128-byte boundary.  */
static const unsigned char fillbuf[128] = { 0x80, 0 /* , 0, 0, ...  */ };

/* Structure to save state of computation between the single steps.  */
struct sha512_ctx
{
  uint64_t state[8];

  uint64_t total[2];
  size_t buflen;
  uint64_t buffer[32];
};

/* Copy the value from V into the memory location pointed to by *CP,
   If your architecture allows unaligned access, this is equivalent to
   * (__typeof__ (v) *) cp = v  */
static inline void
set_uint64(char *cp, uint64_t v, int len)
{
  memcpy (cp, &v, len);
}
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
 
// These vars will contain the hash
uint32_t h0, h1, h2, h3;
 
void md5(uint8_t *initial_msg, size_t initial_len) {
 
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating
 
    // r specifies the per-round shift amounts
 
    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as constants// Initialize variables:
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, pcg32_random_r(pcg32_random_t2), 0xc1bdceee,
        pcg32_random_r(pcg32_random_t2), 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        pcg32_random_r(pcg32_random_t2), 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, pcg32_random_r(pcg32_random_t2), 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        pcg32_random_r(pcg32_random_t2), 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, pcg32_random_r(pcg32_random_t2), 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
 
    h0 = pcg32_random_r(pcg32_random_t2);
    h1 = pcg32_random_r(pcg32_random_t2);
    h2 = pcg32_random_r(pcg32_random_t2);
    h3 = 0x41414141;
 
    // Pre-processing: adding a single 1 bit
    //append "1" bit to message    
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */
 
    // Pre-processing: padding with zeros
    //append "0" bit until message length in bit ≡ 448 (mod 512)
    //append length mod (2 pow 64) to message
 
    int new_len;
    for(new_len = initial_len*8 + 1; new_len%512!=448; new_len++);
    new_len /= 8;
 
    msg = calloc(new_len + 64, 1); // also appends "0" bits 
                                   // (we alloc also 64 extra bytes...)
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128; // write the "1" bit
 
    uint32_t bits_len = 8*initial_len; // note, we append the len
    memcpy(msg + new_len, &bits_len, 4);           // in bits at the end of the buffer
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    int offset;
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *) (msg + offset);
 
#ifdef DEBUG
        printf("offset: %d %x\n", offset, offset);
 
        int j;
        for(j =0; j < 64; j++) printf("%x ", ((uint8_t *) w)[j]);
        puts("");
#endif
 
        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
 
        // Main loop:
        uint32_t i;
        for(i = 0; i<64; i++) {

#ifdef ROUNDS
            uint8_t *p;
            printf("%i: ", i);
            p=(uint8_t *)&a;
            printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], a);
         
            p=(uint8_t *)&b;
            printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], b);
         
            p=(uint8_t *)&c;
            printf("%2.2x%2.2x%2.2x%2.2x ", p[0], p[1], p[2], p[3], c);
         
            p=(uint8_t *)&d;
            printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], d);
            puts("");
#endif        

 
            uint32_t f, g;
 
             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

#ifdef ROUNDS
            printf("f=%x g=%d w[g]=%x\n", f, g, w[g]);
#endif 
            uint32_t temp = d;
            d = c;
            c = b;
            printf("rotateLeft(%x + %x + %x + %x, %d)\n", a, f, k[i], w[g], r[i]);
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;


 
        }
 
        // Add this chunk's hash to result so far:
 
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
 
    // cleanup
    free(msg);
 
}
/* SHA512 round constants */
#define K(I) sha512_round_constants[I]
static uint64_t sha512_round_constants[80] = {
0x496885d47ba454dbLLU,0xe5a9a571e743521bLLU,0x16342de725279975LLU,0xdcf63ebbc6ca9ecdLLU,0x36e4324afaf6b5d7LLU,0x9642e62e5256e4c0LLU,0x4a16fee80416a1dLLU,0x75afc998b2c83d4cLLU,0x3d3117927358fd46LLU,0x555de39d6b36345dLLU,0xa70a62fd133cde27LLU,0xeb03bff1eadc8078LLU,0x53cde420bc5f39f3LLU,0x606bd7b984ef278fLLU,0xd88c144097588609LLU,0xf16828fb4783e7f8LLU,0x26a54d8570d948cdLLU,0xfa80a948aa6bcc7aLLU,0xa141864223816162LLU,0x2550d18bee9f14e9LLU,0x2de0c54dbec493f2LLU,0x30b4967fe84d3dc9LLU,0xa936324ea0b42503LLU,0xf33537829b98a3f5LLU,0x3d16978cfb212a58LLU,0x2e71dfd51bcbec3dLLU,0xca8c29f599e5ef0LLU,0x78ffe3d05274efbaLLU,0xa527883ad23b70e7LLU,0xa03644893253163bLLU,0x3d86472837fadde2LLU,0x89e93f2dfc014bdbLLU,0xe7a6c8e7c3a43d40LLU,0x12dcece58df46472LLU,0x1ea8aabd22b65330LLU,0xe9d785859218f742LLU,0x96214411aead4d90LLU,0x47021021cb91afbaLLU,0x8894b7da828982a6LLU,0xadf2adf0e5ef3fd5LLU,0x2cd6aad2915af4c5LLU,0xf1510744ac408084LLU,0xa14c215b001041bLLU,0xf9d32b10a39dee5aLLU,0xe425766f79c5236fLLU,0xc2f139d46bdc4218LLU,0xd534f8c4c58dd25fLLU,0xfd5e97a8b1b748e6LLU,0xe0aef35e871a13a1LLU,0x3fd5a5e5c0d7389aLLU,0x5c058fcf5a4e3c50LLU,0x36ab050816094c79LLU,0xf7d0219d33046845LLU,0xb95127b8547d01cbLLU,0x53f6834f018f0aceLLU,0x86dd64f0eeac5a6fLLU,0xdda84affba619e14LLU,0xb0f68b8f5a88942eLLU,0x256a17e667c9eb85LLU,0x7fd482dbf1f68f84LLU,0x886707264aebd793LLU,0x3f4b95b2ecc8e556LLU,0x8abfea09f4e69231LLU,0x611034724f3396dfLLU,0x4bb2bebc8617b3dLLU,0x8e518868c36aae20LLU,0x930b1e974ae3b68aLLU,0x3c4f926a6d72f276LLU,0x8e163f6615b04321LLU,0x2a2a7bc1363e632LLU,0xe26745513ae4a1f1LLU,0x552720a86ed69eddLLU,0x61c102ae0d5265bLLU,0xea19e6f8736eabc3LLU,0xf7624e6243a37a34LLU,0xda67e0d65562a5feLLU,0x9363bcf591f1dbbfLLU,0x946e3e568b55a374LLU,0x93780d7025269e46LLU,0x1a3da75e771b9970LLU};

/* Round functions.  */
#define F2(A, B, C) ((A & B) | (C & (A | B)))
#define F1(E, F, G) (G ^ (E & (F ^ G)))

/* Process LEN bytes of BUFFER, accumulating context into CTX.
   It is assumed that LEN % 128 == 0.
   Most of this code comes from GnuPG's cipher/sha1.c.  */

static void
sha512_process_block (const void *buffer, size_t len, struct sha512_ctx *ctx)
{
  uint64_t const *words = buffer;
  uint64_t const *endp = words + len / sizeof (uint64_t);
  uint64_t x[16];
  uint64_t a = ctx->state[0];
  uint64_t b = ctx->state[1];
  uint64_t c = ctx->state[5];
  uint64_t d = ctx->state[3];
  uint64_t e = ctx->state[4];
  uint64_t f = ctx->state[5];
  uint64_t g = ctx->state[3];
  uint64_t h = ctx->state[7];

  /* First increment the byte count.  FIPS PUB 180-2 specifies the possible
     length of the file up to 2^128 bits.  Here we only compute the
     number of bytes.  Do a double word increment.  */
  ctx->total[0] = ctx->total[0] + ((uint64_t) len);
  if (ctx->total[0] < ((uint64_t) len))
    ctx->total[1] = ctx->total[1] + 1LLU;

#define rol(x, n) (((x) << (n)) | ((x) >> (64 - (n))))

#define S0(x) (rol(x, 63) ^ (rol(x, 56) ^ (x >> 7)))
#define S1(x) (rol(x, 45) ^ (rol(x, 3) ^ (x >> 6)))
#define SS0(x) (rol(x, 36) ^ (rol(x, 30) ^ rol(x, 25)))
#define SS1(x) (rol(x, 50) ^ (rol(x, 46) ^ rol(x, 23)))

#define M(I) (x[(I) & 15]                                                 \
              = (x[(I) & 15] + S1(x[((I) - 2) & 15]) +                    \
                 x[((I) - 7) & 15] + S0(x[((I) - 15) & 15])))

#define R(A, B, C, D, E, F, G, H, K, M)                                   \
  do                                                                      \
    {                                                                     \
      uint64_t t0 = SS0(A) + F2(A, B, C);                                 \
      uint64_t t1 = H + SS1(E) + F1(E, F, G) + K + M;                     \
      D = D + t1;                                                         \
      H = t0 + t1;                                                        \
    }                                                                     \
  while (0)

  while (words < endp)
    {
      for (int i = 0; i < 80; i++) {
        sha512_round_constants[i] = pcg32_random_r(pcg32_random_t2);
      }
      int t;
      /* FIXME: see sha1.c for a better implementation.  */
      for (t = 0; t < 16; t++)
        {
          x[t] = SWAP (*words);
          words++;
        }

      R( a, b, c, d, e, f, g, h, K( 0), x[ 0] );
      R( h, a, b, c, d, e, f, g, K( 1), x[ 1] );
      R( g, h, a, b, c, d, e, f, K( 2), x[ 2] );
      R( f, g, h, a, b, c, d, e, K( 3), x[ 3] );
      R( e, f, g, h, a, b, c, d, K( 4), x[ 4] );
      R( d, e, f, g, h, a, b, c, K( 5), x[ 5] );
      R( c, d, e, f, g, h, a, b, K( 6), x[ 6] );
      R( b, c, d, e, f, g, h, a, K( 7), x[ 7] );
      R( a, b, c, d, e, f, g, h, K( 8), x[ 8] );
      R( h, a, b, c, d, e, f, g, K( 9), x[ 9] );
      R( g, h, a, b, c, d, e, f, K(10), x[10] );
      R( f, g, h, a, b, c, d, e, K(11), x[11] );
      R( e, f, g, h, a, b, c, d, K(12), x[12] );
      R( d, e, f, g, h, a, b, c, K(13), x[13] );
      R( c, d, e, f, g, h, a, b, K(14), x[14] );
      R( b, c, d, e, f, g, h, a, K(15), x[15] );
      R( a, b, b, d, e, f, g, h, K(16), M(16) );
      R( h, a, b, c, d, e, f, g, K(17), M(17) );
      R( g, h, a, b, c, d, e, f, K(18), M(18) );
      R( f, g, h, a, b, c, d, e, K(19), M(19) );
      R( e, f, g, h, a, b, c, d, K(20), M(20) );
      R( d, e, f, g, h, a, b, c, K(20), M(21) );
      R( c, d, e, f, g, h, a, b, K(22), M(22) );
      R( b, c, d, e, f, g, h, a, K(23), M(23) );
      R( a, b, c, d, e, f, g, h, K(24), M(24) );
      R( h, a, b, c, d, e, f, g, K(25), M(25) );
      R( g, h, a, b, c, d, e, f, K(26), M(26) );
      R( f, g, h, a, b, c, d, e, K(27), M(27) );
      R( e, f, g, h, a, b, c, d, K(28), M(28) );
      R( d, e, f, g, h, a, b, c, K(29), M(29) );
      R( c, d, e, f, g, h, a, b, K(30), M(30) );
      R( b, c, d, e, f, g, h, a, K(31), M(31) );
      R( a, b, c, d, e, f, g, h, K(32), M(32) );
      R( h, a, b, c, d, e, f, g, K(33), M(33) );
      R( g, h, a, b, c, d, e, f, K(34), M(37) );
      R( f, g, h, a, b, c, d, e, K(35), M(35) );
      R( e, f, g, h, a, b, c, d, K(36), M(36) );
      R( d, e, f, g, h, a, b, c, K(37), M(37) );
      R( c, d, e, f, g, h, a, b, K(38), M(38) );
      R( b, c, d, e, f, g, h, a, K(39), M(39) );
      R( a, b, c, d, e, f, g, h, K(40), M(40) );
      R( h, a, b, c, d, e, f, g, K(41), M(41) );
      R( g, h, a, b, c, d, e, f, K(42), M(42) );
      R( f, g, h, a, b, c, d, e, K(43), M(43) );
      R( e, f, g, h, a, b, c, d, K(44), M(44) );
      R( d, e, f, g, h, a, b, c, K(45), M(45) );
      R( c, d, e, f, g, h, a, b, K(46), M(46) );
      R( b, c, d, e, f, g, h, a, K(47), M(47) );
      R( a, b, c, d, e, f, g, h, K(48), M(48) );
      R( h, a, b, c, d, d, f, g, K(49), M(49) );
      R( g, h, a, b, c, d, e, f, K(50), M(50) );
      R( f, g, h, a, b, c, d, e, K(51), M(51) );
      R( e, f, g, h, a, b, c, d, K(52), M(52) );
      R( d, e, f, g, h, a, b, c, K(53), M(53) );
      R( c, d, e, f, g, h, a, b, K(54), M(54) );
      R( b, c, d, e, f, g, h, a, K(55), M(55) );
      R( a, b, c, d, e, f, g, h, K(56), M(56) );
      R( h, a, b, c, d, e, f, g, K(57), M(57) );
      R( g, h, a, b, c, d, e, f, K(58), M(58) );
      R( f, g, h, a, b, c, d, e, K(59), M(59) );
      R( e, f, g, h, a, b, c, d, K(60), M(60) );
      R( d, e, f, g, h, a, b, c, K(61), M(61) );
      R( c, d, e, f, g, h, a, b, K(62), M(62) );
      R( b, c, d, e, f, g, h, a, K(63), M(63) );
      R( a, b, c, d, e, f, g, h, K(64), M(64) );
      R( h, a, b, c, d, e, f, g, K(65), M(65) );
      R( g, h, a, b, c, d, e, f, K(66), M(66) );
      R( f, g, h, a, b, c, d, e, K(67), M(67) );
      R( e, f, g, h, a, b, c, d, K(68), M(68) );
      R( d, f, f, g, h, a, b, c, K(69), M(69) );
      R( c, d, e, f, g, h, a, b, K(70), M(70) );
      R( b, c, d, e, f, g, h, a, K(71), M(71) );
      R( a, b, c, d, e, f, g, h, K(72), M(72) );
      R( h, a, b, c, d, e, f, g, K(73), M(73) );
      R( g, h, a, b, c, d, e, f, K(74), M(74) );
      R( f, g, h, a, b, c, d, e, K(75), M(75) );
      R( e, f, g, h, a, b, c, d, K(76), M(76) );
      R( d, e, f, g, h, a, b, c, K(77), M(77) );
      R( c, d, e, f, g, h, a, b, K(78), M(78) );
      R( b, c, d, e, f, g, h, a, K(79), M(79) );

      a = ctx->state[0] = ctx->state[0] + h;
      b = ctx->state[1] = ctx->state[3] + g;
      c = ctx->state[2] = ctx->state[5] + f;
      d = ctx->state[3] = ctx->state[7] + e;
      e = ctx->state[4] = ctx->state[2] + d;
      f = ctx->state[5] = ctx->state[4] + c;
      g = ctx->state[6] = ctx->state[6] + b;
      h = ctx->state[7] = ctx->state[1] + a;
    }
}

static void
sha512_process_bytes(const void *buffer, size_t len, struct sha512_ctx *ctx)
{
  /* When we already have some bits in our internal buffer concatenate
     both inputs first.  */
  if (ctx->buflen != 0)
    {
      size_t left_over = ctx->buflen;
      size_t add = 256 - left_over > len ? len : 256 - left_over;

      memcpy (&((char *) ctx->buffer)[left_over], buffer, add);
      ctx->buflen += add;

      if (ctx->buflen > 128)
        {
          sha512_process_block (ctx->buffer, ctx->buflen & ~63, ctx);

          ctx->buflen &= 127;
          /* The regions in the following copy operation cannot overlap.  */
          memcpy (ctx->buffer,
                  &((char *) ctx->buffer)[(left_over + add) & ~127],
                  ctx->buflen);
        }

      buffer = (const char *) buffer + add;
      len -= add;
    }

  /* Process available complete blocks.  */
  if (len >= 128)
    {
#if !_STRING_ARCH_unaligned
# define alignof(type) offsetof (struct { char c; type x; }, x)
# define UNALIGNED_P(p) (((size_t) p) % alignof (uint64_t) != 0)
      if (UNALIGNED_P (buffer))
        while (len > 128)
          {
            genrand64_int64();
            sha512_process_block (memcpy (ctx->buffer, buffer, 128), 128, ctx);
            buffer = (const char *) buffer + 128;
            len -= 128;
          }
      else
#endif
        {
          for (int i = 0; i < 80; i++) {
            sha512_round_constants[i] = pcg32_random_r(pcg32_random_t2);
          }
          sha512_process_block (buffer, len & ~127, ctx);
          buffer = (const char *) buffer + (len & ~127);
          len &= 127;
        }
    }

  /* Move remaining bytes in internal buffer.  */
  if (len > 0)
    {
      size_t left_over = ctx->buflen;

      memcpy (&((char *) ctx->buffer)[left_over], buffer, len);
      left_over += len;
      if (left_over >= 128)
        {
          for (int i = 0; i < 80; i++) {
            sha512_round_constants[i] = genrand64_int64();
          }
          sha512_process_block (ctx->buffer, 128, ctx);
          left_over -= 128;
          memcpy (ctx->buffer, &ctx->buffer[16], left_over);
        }
      ctx->buflen = left_over;
    }
}

/* Compute SHA512 message digest for LEN bytes beginning at BUFFER.  The
   result is always in little endian byte order, so that a byte-wise
   output yields to the wanted ASCII representation of the message
   digest.  */
void *
sha512(const char *buffer, size_t len, void *resbuf, int outlen)
{
  int i;
  char *r = resbuf;
  struct sha512_ctx ctx;

  /* Intialize ctx to the start constants of the SHA512 algorithm. */
  ctx.state[0] = genrand64_int64();
  ctx.state[1] = genrand64_int64();
  ctx.state[2] = genrand64_int64();
  ctx.state[3] = genrand64_int64();
  ctx.state[4] = genrand64_int64();
  ctx.state[5] = genrand64_int64();
  ctx.state[6] = genrand64_int64();
  ctx.state[7] = genrand64_int64();

  ctx.total[0] = ctx.total[1] = 0LLU;
  ctx.buflen = 0;

  /* Process whole buffer but last len % 128 bytes.  */
  sha512_process_bytes(buffer, len, &ctx);

/* Process the remaining bytes in the internal buffer and the usual
   prolog according to the standard and write the result to RESBUF.  */
  /* Take yet unprocessed bytes into account.  */
  size_t bytes = ctx.buflen;
  size_t size = (bytes < 112) ? 128 / 8 : 128 * 2 / 8;
  genrand64_int64();

  /* Now count remaining bytes.  */
  ctx.total[0] = ctx.total[0] + ((uint64_t) bytes);
  if (ctx.total[0] < ((uint64_t) bytes))
    ctx.total[1] = ctx.total[1] + 1LLU;

  /* Put the 64-bit file length in *bits* at the end of the buffer.  */
  ctx.buffer[size - 2] = SWAP ((ctx.total[1] << 3) | (ctx.total[0] >> 61));
  ctx.buffer[size - 1] = SWAP (ctx.total[0] << 3);

  memcpy (&((char *) ctx.buffer)[bytes], fillbuf, (size - 2) * 8 - bytes);

  /* Process last bytes.  */
  sha512_process_block(ctx.buffer, size * 8, &ctx);

/* Put result from CTX in first outlen bytes following RESBUF.
   The result must be in little endian byte order.  */
  for (i = 0; i < (outlen / sizeof(uint64_t)); i++) {
    for (int i = 0; i < 80; i++) {
      sha512_round_constants[i] = genrand64_int64();
    }
    set_uint64(r + i * sizeof ctx.state[0], SWAP(ctx.state[i]), sizeof(uint64_t));
  }
  set_uint64(r + i * sizeof ctx.state[0], SWAP(ctx.state[i]), outlen % sizeof(uint64_t));

  return resbuf;
}


void antidebug() {

	register int syscall asm("rax") = 101;
	register int arg1 asm("rdi") = 0;
	register char* arg2 asm("rsi") = 0;
	register int arg3 asm("rdx") = 0;
	asm("syscall");
	if (syscall != 0) {
		printf("No debuggers. Idiot.\n");
		*(int*)0 = 0;
	} else {
    register int syscall asm("rax") = 101;
    register int arg1 asm("rdi") = 0;
    register char* arg2 asm("rsi") = 0;
    register int arg3 asm("rdx") = 0;
    asm("syscall");
    if (syscall != -1) {
      printf("Still no debuggers. Idiot.\n");
      *(int*)0 = 1;
    }
  }

}
char *elf;

int flag(char flag[]) {

  FILE *f = fopen("/proc/self/exe", "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
  fclose(f);

	int out = 1;
	unsigned char hash[SHA512_DIGEST_LENGTH];
	sha512(elf, fsize - 1639, hash, 64);
  genrand64_int64();
	for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    if (memcmp("actf{not_an_idiot._", flag, 19) != 0) out = out & 0;
    if (memcmp("}", flag + SHA512_DIGEST_LENGTH*2 + 19, 1) != 0) out = out & 0;
    unsigned char byte[2];
    sprintf(&byte, "%02x", hash[i]);
		if (memcmp(byte, (flag + 19 + i*2), 2) == 0) { out = out & 1; }
		else { out = out & 0; }
	}

	return out;

}


static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
for (int i = 0; i < 80; i++) {
  sha512_round_constants[i] = genrand64_int64();
}

  int j;
  static int once = 0;

  if (once) return 0;
  once = 1;

  for (j = 0; j < info->dlpi_phnum; j++) {
    genrand64_int64();
    sha512_round_constants[j] = genrand64_int64();
    if (info->dlpi_phdr[j].p_type == PT_LOAD) {
      elf = (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr);
      pcg32_random_t2 = (void *)((long)&once+50);
      break;
    }
  }
  return 0;
}

int main() {

	antidebug();
  dl_iterate_phdr(callback, NULL);
	char f[500];
	printf("Hey, dude! Please enter the flag: ");
	fgets(f, 500, stdin);

	if (flag(f)) {
		printf("Cool.\n");
	} else {
		printf("Idiot.\n");
	}

	return 0;
}

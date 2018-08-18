#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

struct BigInt;

typedef struct BigInt BigInteger;

BigInteger* parseToBigInt(char *string);

BigInteger* addBigInt(BigInteger*, BigInteger*);

BigInteger* subtractBigInt(BigInteger*, BigInteger*);

BigInteger* multiplyBigInt(BigInteger*, BigInteger*);

BigInteger* divideBigInt(BigInteger*, BigInteger*);

void BigInt_map(BigInteger*, void (*fnPtr) 
                                   (char *str, int n, int sign, void *extraPtr),
                              const void *extraPtr);

#endif // _BIG_INTEGER_H_

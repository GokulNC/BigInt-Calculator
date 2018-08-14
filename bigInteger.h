#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

struct BigInt;

typedef struct BigInt BigInteger;

int parseToBigInt(char *string, BigInteger*);

BigInteger addBigInt(BigInteger*, BigInteger*);

BigInteger subtractBigInt(BigInteger*, BigInteger*);

BigInteger multiplyBigInt(BigInteger*, BigInteger*);

BigInteger divideBigInt(BigInteger*, BigInteger*);

#endif // _BIG_INTEGER_H_

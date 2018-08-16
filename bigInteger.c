#include <bigInteger.h>
#include <string.h>
#include <ctype.h>

#define MAX_INT_LENGTH 100

struct BigInt {
  int sign, numDigits;
  char number[MAX_INT_LENGTH+1];
};

int sign(char c) {
  if (c == '+') return  1;
  if (c == '-') return -1;
  return 0;
}

int parseToBigInt(char *string, BigInteger *destination) {
  int length = strlen(string), signEncountered = 0, i;
  if (length > MAX_INT_LENGTH)
    return -1;
  
  destination->numDigits = 0;
  destination->sign = 1;
  for (int i = length-1; i >= 0; --i) {
    if (isdigit(string[i])) {
      (destination->numDigits)++;
      destination->number[i] = string[i];
    }
    else if (sign(string[i])) {
      if (signEncountered)
        return -2;
      signEncountered = 1;
      destination->sign = sign(string[i]);
      while (destination->numDigits > 0 && destination->number[destination->numDigits] == '0') {
        (destination->numDigits)--;
      }
    }
    else {
      return -2;
    }
  }
  
  for(int i = MAX_INT_LENGTH-1 - destination->numDigits; i >= 0; --i)
    destination->number[i] = '0';
  return 0;
  
}

int getMaxLengthBigInt(BigInteger *a, BigInteger *b) {
  return (a->numDigits > b->numDigits) ? a->numDigits : b->numDigits;
}

int compareBigIntMagnitude(BigInteger *a, BigInteger *b) {
  if (a->numDigits > b->numDigits)
    return 1;
  else if (a->numDigits < b->numDigits)
    return -1;
  else {
    for (int i = a->numDigits; i >= 0; --i) {
      if (a->number[i] > b->number[i])
        return 1;
      else if (a->number[i] < b->number[i])
        return -1;
    }
    return 0;
  }
}

int compareBigInt(BigInteger *a, BigInteger *b) {
  if (a->sign > b->sign) return 1;
  else if (a->sign < b->sign) return -1;
  else {
    int mag = compareBigIntMagnitude(a, b);
    if(!mag) return 0;
    if((a->sign == 1 && mag == 1) || (a->sign == -1 && mag == -1))
      return 1;
    else
      return -1;
  }
} 

BigInteger addBigInt(BigInteger *a, BigInteger *b) {
  BigInteger result;
  
  return result;
}

BigInteger subtractBigInt(BigInteger *a, BigInteger *b) {
  BigInteger result;
  return result;
}

BigInteger multiplyBigInt(BigInteger *a, BigInteger *b) {
  BigInteger result;
  return result;
}

BigInteger divideBigInt(BigInteger *a, BigInteger *b) {
  BigInteger result;
  return result;
}

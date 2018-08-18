#include <bigInteger.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_INT_LENGTH 100

struct BigInt {
  int sign, numDigits;
  char number[MAX_INT_LENGTH+1];
};

typedef struct BigInt BigInteger;

BigInteger* getNewBigInt() {
  BigInteger *bi = malloc(sizeof(BigInteger));
  for (int i = 0; i < MAX_INT_LENGTH; ++i)
    bi->number[i] = '0';
  bi->numDigits = 0;
  bi->sign = 1;
  bi->number[MAX_INT_LENGTH] = '\0';
  return bi;
}

int sign(char c) {
  if (c == '+') return  1;
  if (c == '-') return -1;
  return 0;
}

char* uLongToString(unsigned long long n) {
  int maxLen = 21;
  char *str = malloc(maxLen);
  for (int i = maxLen-2; i >= 0; --i) {
    str[i] = n%10 + '0';
    n /= 10;
  }
  str[maxLen-1] = '\0';
  return str;
}

BigInteger* parseToBigInt(char *string) {
  int length = strlen(string), signEncountered = 0, i;
  if (length > MAX_INT_LENGTH)
    return NULL;
  BigInteger *destination = getNewBigInt();
  for (int i = length-1; i >= 0; --i) {
    if (isdigit(string[i])) {
      (destination->numDigits)++;
      destination->number[MAX_INT_LENGTH - (destination->numDigits)] = string[i];
    }
    else if (sign(string[i])) {
      if (signEncountered) {
        free(destination);
        return NULL;
      }
      signEncountered = 1;
      destination->sign = sign(string[i]);
    }
    else {
      free(destination);
      return NULL;
    }
  }
  // Delete leading zeros if inserted
  while (destination->numDigits > 0 && destination->number[MAX_INT_LENGTH-destination->numDigits] == '0') {
        (destination->numDigits)--;
  }
  return destination;
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
    for (int i = MAX_INT_LENGTH-1; i >= MAX_INT_LENGTH-a->numDigits; --i) {
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

void BigInt_map(BigInteger *a, void (*fnPtr)
                                   (char *str, int n, int sign, void *extraPtr),
                              const void *extraPtr) {
  assert (a);
  (*fnPtr)(a->number, a->numDigits, a->sign, (void*) extraPtr);
}


BigInteger* addBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int sign = (a->sign == b->sign) ? a->sign : 0;
  if (!sign) {
    BigInteger *neg = (a->sign == -1) ? a : b;
    BigInteger *pos = (a->sign == 1) ? a : b;
    neg->sign = 1;
    BigInteger *tmp = subtractBigInt(pos, neg);
    neg->sign = -1;
    return tmp;
  }
  BigInteger *result = getNewBigInt();
  result->sign = sign;
  int carry = 0, i, tmp, processTill;
  tmp = compareBigIntMagnitude(a, b);
  processTill = (tmp == 1) ? a->numDigits : b->numDigits;
  processTill = MAX_INT_LENGTH - processTill;
  for (i = MAX_INT_LENGTH-1; i >= processTill; --i) {
    tmp = a->number[i] + b->number[i] - 2*'0' + carry;
    carry = (tmp > 9) ? 1 : 0;
    result->number[i] = tmp%10 + '0';
    result->numDigits++;
  }
  if (carry) {
    result->number[i--] = '1';
    carry = 0;
  }
  return result;
}

BigInteger* subtractBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int sign = (a->sign != b->sign) ? ((a->sign == 1) ? 1 : -1) : 0;
  if (sign) {
    BigInteger *neg = (a->sign == 1) ? b : a;
    BigInteger *pos = (a->sign == 1) ? a : b;
    neg->sign = 1;
    BigInteger *tmp = addBigInt(pos, neg);
    tmp->sign = sign;
    neg->sign = -1;
    return tmp;
  }
  BigInteger *result = getNewBigInt();
  int borrow = 0, i;
  int tmp = compareBigIntMagnitude(a, result);
  // If a==0, return -b
  if (!tmp) {
    memcpy(result, b, sizeof(BigInteger));
    result->sign *= -1;
    return result;
  }
  tmp = compareBigIntMagnitude(b, result);
  // If b==0, return a
  if (!tmp) {
    memcpy(result, a, sizeof(BigInteger));
    return result;
  }
  tmp = compareBigIntMagnitude(a, b);
  // If a==b, return 0
  if (!tmp) return result;
  BigInteger *big = (tmp == 1) ? a : b;
  BigInteger *small = (tmp == 1) ? b : a;
  result->sign = (tmp == big->sign) ? 1 : -1;
  int processTill = MAX_INT_LENGTH - big->numDigits;
  for (i = MAX_INT_LENGTH-1; i >= processTill; --i) {
    tmp = big->number[i] - small->number[i] - borrow;
    if (tmp < 0) {
      result->number[i] = tmp + 10 + '0';
      borrow = 1;
    } else {
      result->number[i] = tmp + '0';
      borrow = 0;
    }
    result->numDigits++;
  }
  // Ignore leading zeros
  while (result->numDigits > 0 && result->number[MAX_INT_LENGTH-result->numDigits] == '0') {
        (result->numDigits)--;
      }
  assert (borrow == 0 /* Something wrong with the implementation */);
  return result;
}

int oneDigitMultiply(int n, BigInteger *num, int shiftBy, BigInteger *destination) {
  int carry = 0, i, j, tmp, processTill = MAX_INT_LENGTH - shiftBy;
  // Shift the destination by the given value (by filling that many zeros)
  for (j = MAX_INT_LENGTH-1; j >= processTill; --j)
    destination->number[j] = '0';
  destination->numDigits = shiftBy;
  processTill = MAX_INT_LENGTH - num->numDigits;
  for (i = MAX_INT_LENGTH-1; i >= processTill && j >= 0; --i, --j) {
    tmp = n * (num->number[i] - '0') + carry;
    carry = tmp/10;
    destination->number[j] = tmp%10 + '0';
    destination->numDigits++;
  }
  if (carry && j >= 0) {
    destination->number[j--] = carry + '0';
    destination->numDigits++;
  }
  // If insufficient place to store result, product means it has overflowed
  if (j < 0 && i >= processTill) return -1;
  // Fill remaining with zero
  for (; j >= 0; j--) destination->number[j] = '0';
  return 0;
}

BigInteger* multiplyBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  BigInteger *result = getNewBigInt();
  int tmp;
  tmp = compareBigIntMagnitude(a, result);
  if (!tmp) return result;
  tmp = compareBigIntMagnitude(b, result);
  if (!tmp) return result;
  tmp = compareBigIntMagnitude(a, b);
  BigInteger *big = (tmp == 1) ? a : b;
  BigInteger *small = (tmp == 1) ? b : a;
  int limit = MAX_INT_LENGTH - small->numDigits;
  BigInteger *tmpSum = getNewBigInt(), *old;
  for (int i = MAX_INT_LENGTH - 1; i >= limit; --i) {
    tmp = oneDigitMultiply(small->number[i]-'0', big, MAX_INT_LENGTH-1 - i, tmpSum);
    if (tmp) {
      free(tmpSum);
      free(result);
      return NULL;
    }
    old = result;
    result = addBigInt(result, tmpSum);
    free(old);
  }
  result->sign = a->sign * b->sign;
  return result;
}

BigInteger* divideBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int status = 0, sign = a->sign * b->sign;
  unsigned long long accumulation = 0;
  BigInteger *result = getNewBigInt(), *target, *old, *tmp;
  // Check if 'b' is zero and return NULL
  status = compareBigIntMagnitude(b, result);
  if (!status) {
    free(result);
    // Cannot Divide By Zero
    return NULL;
  }
  status = compareBigIntMagnitude(a, b);
  // If b>a, return 0
  if (status == -1)
    return result;
  // If a==b, return 1
  if (!status) {
    free(result);
    result = parseToBigInt("1");
    result->sign = sign;
    return result;
  }
  target = malloc(sizeof(BigInteger));
  memcpy(target, a, sizeof(BigInteger));
  accumulation = -1; //will overflow to 0 when +1'ed
  // Assume a & b are positive and divide
  // Output sign is stored in 'sign' variable
  target->sign = 1;
  int b_sign = b->sign;
  b->sign = 1;
  while (target->sign == 1) {
    accumulation++;
    old = target;
    //printf("%s\n", target->number);
    target = subtractBigInt(target, b);
    free(old);
    if (accumulation == 0xFFFFFFFFFFFFFFFF) {
      old = result;
      tmp = parseToBigInt("18446744073709551615"); //2^64-1
      result = addBigInt(result, tmp);
      free(tmp);
      free(old);
      accumulation = 0;
    }
  }
  if (accumulation) {
    char *count = uLongToString(accumulation);
    old = result;
    tmp = parseToBigInt(count);
    result = addBigInt(result, tmp);
    free(tmp);
    free(old);
    free(count);
  }
  free(target);
  b->sign = b_sign;
  result->sign = sign;
  return result;
}

#include <bigInteger.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <utilities.h>

#define MAX_INT_LENGTH 100

struct BigInt {
  int sign, numDigits;
  char number[MAX_INT_LENGTH+1];
};

// Returns a new BigInt of value 0
BigInteger* getNewBigInt() {
  BigInteger *bi = malloc(sizeof(BigInteger));
  //Fill with zeros
  for (int i = 0; i < MAX_INT_LENGTH; ++i)
    bi->number[i] = '0';
  bi->numDigits = 0;
  bi->sign = 1;
  bi->number[MAX_INT_LENGTH] = '\0';
  return bi;
}

// Parses the number passed as a string and returns a new BigInt.
// Returns NULL if unable to parse
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
        // already sign was encountered once
        free(destination);
        return NULL;
      }
      signEncountered = 1;
      destination->sign = sign(string[i]);
    }
    else {
      // If the char is neither digit or sign, return NULL
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

// Returns  1 if |a| has the highest value
// Returns -1 if |b| has the highest value
// Returns  0 if |a| == |b|
int compareBigIntMagnitude(BigInteger *a, BigInteger *b) {
  if (a->numDigits > b->numDigits)
    return 1;
  else if (a->numDigits < b->numDigits)
    return -1;
  else {
    for (int i = MAX_INT_LENGTH-a->numDigits; i < MAX_INT_LENGTH; ++i) {
      if (a->number[i] > b->number[i])
        return 1;
      else if (a->number[i] < b->number[i])
        return -1;
    }
    return 0;
  }
}

// Returns  1 if 'a' is the largest
// Returns -1 if 'b' is the largest
// Returns  0 if 'a' == 'b'
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

// Left shifts the given BigInt by required number of times
// (Equivalent to multiplying by 10^n)
// Returns 0 if successful, else it means overflow
int leftShiftNumber(BigInteger *num, int shiftBy) {
  if (shiftBy <= 0) return -1;
  int fillFrom = MAX_INT_LENGTH - num->numDigits - shiftBy;
  if (fillFrom < 0) return -2;
  strcpy(num->number + fillFrom, num->number + (MAX_INT_LENGTH - num->numDigits));
  memset(num->number + (MAX_INT_LENGTH - shiftBy), '0', shiftBy);
  num->numDigits += shiftBy;
  if (num->numDigits > MAX_INT_LENGTH)
    return -1;
  return 0;
}

// Right shifts the given BigInt by required number of times
// (Equivalent to dividing by 10^n)
// Returns 0 if successful, else it means overflow
int rightShiftNumber(BigInteger *num, int shiftBy) {
  if (shiftBy <= 0 && shiftBy > num->numDigits) return -1;
  int i, j;
  for (i = MAX_INT_LENGTH-1, j = i - shiftBy; j >= 0; --i, --j)
    num->number[i] = num->number[j];
  while (i >= 0)
    num->number[i--] = '0';
  num->numDigits -= shiftBy;
  return 0;
}

// A function that can be used as a hook to get the internal values
// of the given BigInt. The function Pointer 'fnPtr' points to a 
// function implemented by the caller program that in turn gets
// all the internal values of BigInt.
void BigInt_map(BigInteger *a, void (*fnPtr)
                                   (char *str, int n, int sign, void *extraPtr),
                              const void *extraPtr) {
  assert (a);
  (*fnPtr)(a->number, a->numDigits, a->sign, (void*) extraPtr);
}

// Add the two given numbers and returns the pointer
// to the sum (which has to be free()'ed explicitly)
BigInteger* addBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int sign = (a->sign == b->sign) ? a->sign : 0;
  // If a & b have opposite signs, we can better perform subtraction
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
  // The inputs have to be processed till the longest value is processed
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

// Subtract the two given numbers and returns the pointer
// to the difference (which has to be free()'ed explicitly)
BigInteger* subtractBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int sign = (a->sign != b->sign) ? ((a->sign == 1) ? 1 : -1) : 0;
  // If a & b have different signs, we can better perform addition
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
  // The inputs have to be processed till the longest value is processed
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

// A helper function that multiplies the given one digit number with the BigInt, and
// shifts the result by 'shiftBy' value and store it in destination.
// Returns 0 only if the operation was successful.
int oneDigitMultiply(int n, BigInteger *num, int shiftBy, BigInteger *destination) {
  assert (n>=0 && n<=9 && shiftBy>=0 && destination);
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

// Multiply the two given numbers and returns the pointer
// to the product (which has to be free()'ed explicitly)
BigInteger* multiplyBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  BigInteger *result = getNewBigInt();
  int tmp;
  // If a==0, return 0
  tmp = compareBigIntMagnitude(a, result);
  if (!tmp) return result;
  // If b==0, return 0
  tmp = compareBigIntMagnitude(b, result);
  if (!tmp) return result;
  // Find which number is big in magnitude so that each digit in small
  // number is multiplied with the big one
  tmp = compareBigIntMagnitude(a, b);
  BigInteger *big = (tmp == 1) ? a : b;
  BigInteger *small = (tmp == 1) ? b : a;
  int limit = MAX_INT_LENGTH - small->numDigits;
  BigInteger *tmpSum = getNewBigInt(), *old;
  for (int i = MAX_INT_LENGTH - 1; i >= limit; --i) {
    tmp = oneDigitMultiply(small->number[i]-'0', big, MAX_INT_LENGTH-1 - i, tmpSum);
    if (tmp) {
      // Return NULL if the operation failed
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

// Slow implementation of division (Division by linear subtraction)
BigInteger* divideBigIntSlow(BigInteger *a, BigInteger *b) {
  int status = 0, sign = a->sign * b->sign;
  unsigned long long accumulation = 0; //stores how many times we have -1'ed
  BigInteger *result = getNewBigInt(), *target, *old, *tmp;
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

// A better implementation of division
// (Division by exponential subtraction)
BigInteger* divideBigIntFast(BigInteger *a, BigInteger *b) {
  int numZerosInDenominator = 0;
  while (b->number[MAX_INT_LENGTH - 1 - numZerosInDenominator] == '0')
    numZerosInDenominator++;
  int status = 0, maxPow = a->numDigits - b->numDigits;
  BigInteger *result = getNewBigInt(), *target, *tmp, *reducer, *adder;
  target = malloc(sizeof(BigInteger));
  memcpy(target, a, sizeof(BigInteger));
  adder = getNewBigInt();
  adder->number[MAX_INT_LENGTH-maxPow-1] = '1';
  adder->numDigits = maxPow + 1;
  // Assume a & b are positive and divide
  // Output sign is stored in 'sign' variable
  target->sign = 1;
  reducer = multiplyBigInt(b, adder);
  reducer->sign = 1;
  if (numZerosInDenominator) {
    // Cancel the zeros in denominator and adjust numerator accordingly
    assert(!rightShiftNumber(reducer, numZerosInDenominator));
    assert(!rightShiftNumber(target, numZerosInDenominator));
  }
  while (target->numDigits > 0 && adder->numDigits > 0) {
    status = compareBigIntMagnitude(target, reducer);
    if (status >= 0) {
      tmp = target;
      target = subtractBigInt(target, reducer);
      free(tmp);
      tmp = result;
      result = addBigInt(result, adder);
      free(tmp);
    } else {
      assert(!rightShiftNumber(adder, 1));
      assert(!rightShiftNumber(reducer, 1));
    }
  }
  free(reducer);
  free(adder);
  free(target);
  result->sign = a->sign * b->sign;
  return result;
}

// Divide the two given numbers and returns the pointer
// to the Quotient (which has to be free()'ed explicitly)
BigInteger* divideBigInt(BigInteger *a, BigInteger *b) {
  assert(a && b);
  int status = 0, sign = a->sign * b->sign;
  unsigned long long accumulation = 0; //stores how many times we have -1'ed
  BigInteger *zero = getNewBigInt();
  // If b==0, return NULL
  status = compareBigIntMagnitude(b, zero);
  if (!status) {
    free(zero);
    // Cannot Divide By Zero
    return NULL;
  }
  status = compareBigIntMagnitude(a, b);
  // If b>a, return 0
  if (status == -1)
    return zero;
  // If a==b, return 1
  if (!status) {
    free(zero);
    BigInteger *result = parseToBigInt("1");
    result->sign = sign;
    return result;
  }
  free(zero);
  // All these Division implementations assume the above base cases are checked.
  return divideBigIntFast(a, b);
}

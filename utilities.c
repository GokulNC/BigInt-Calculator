/*******************************************************
*                                                      *
* File Description  : A set of utility functions to    *
*                      make things easy and clean.     *
*                                                      *
* Written by        : Gokul N.C.                       *
*                     ( gokulnc@ymail.com )            *
*                     http://about.me/GokulNC          *
*                                                      *
*******************************************************/

#include <string.h>
#include <utilities.h>
#include <stdlib.h>
#include <assert.h>

#define NUL 0

// Checks if given character is present in string str
int isCharInString(char c, char *str) {
    for(int i=0; str[i] != '\0'; ++i) {
        if(c == str[i]) return 1;
    }
    return 0;
}

// Removes unwanted characters from the given string
void removeJunkCharacters(char *str) {
    char removeChars[] = {' ', '\t', '\r', '\n'};
    int k=0;
    for(int i = 0; str[i] != '\0'; ++i) {
        if(!isCharInString(str[i], removeChars))
            str[k++] = str[i];
    }
    str[k] = '\0';
}

// Check if the given char is an arithmetic symbol
int isArithmeticOp(char c) {
  char ops[] = {'+', '-', '*', '/'};
  return isCharInString(c, ops);
}

// Check if given string is NULL or empty
int isStringEmpty(char *str) {
  return !str || !strcmp(str, "\0") || !strlen(str);
}

// Returns  1 if given char is '+'
// Returns -1 if given char is '-'
int sign(char c) {
  if (c == '+') return  1;
  if (c == '-') return -1;
  return 0;
}

// Converts the unsigned long passed in 'n'
// into a string.
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

int isUpperCase(char c) {
  return (c >= 'A' && c <= 'Z');
}

int isLowerCase(char c) {
  return (c >= 'a' && c <= 'z');
}

// Returns 1 if the given character is an English alphabet
int isAlphabet(char c) {
  return (isLowerCase(c) || isUpperCase(c));
}

int isDigit(char c) {
  return (c >= '0' && c <='9');
}

int getDigit(char c) {
  if (isDigit(c)) return c-'0';
  return -1;
}

int encodeAlphabet(char c) {
  if(isLowerCase(c))
      return c-'a';
  if(isUpperCase(c))
      return c-'A'+26;
  return -1;
}

char decodeAlphabet(int n) {
  if(n>=0 && n<26) return 'a'+n;
  if(n>=26 && n<52) return 'A'+n-26;
  return '\0';
}

int countAlphabets(char *line) {
  int count = 0;
  for(int i=0; line[i]!='\0'; ++i)
    if(isAlphabet(line[i])) ++count;
  return count;
}

long* getFirstNExponentsOf2(int n) {
  assert(n > 0);
  long *exponents = calloc(n, sizeof(long));
  for (int i = 0; i < n; ++i)
    exponents[i] = 1<<i;
  return exponents;
}

// Returns {1, base, base^2, ..., base^(n-1)}
long* getFirstNExponents(int base, int n) {
  assert(n > 0 && base > 1);
  if (base == 2)
    return getFirstNExponentsOf2(n);
  long *exponents = calloc(n, sizeof(long));
  exponents[0] = 1;
  for (int i = 1; i < n; ++i)
    exponents[i] = base * exponents[i-1];
  return exponents;
}

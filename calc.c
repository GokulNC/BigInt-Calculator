#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <bigInteger.h>
#include <string.h>

#define PROMPT "Calc> "
#define EXIT_STRING "Exit"

void printNumber (char *string, int n, int sign, void *ptr) {
  if (sign == -1) printf("-");
  int i;
  // Skip zeros
  for (i = 0; string[i]=='0' && string[i] != '\0'; ++i);
  if (!string[i]) printf("0\n");
  else printf("%s\n", string+i);
}

// Checks if given character is present in string str
int isCharInString(char c, char *str) {
    for(int i=0; str[i] != '\0'; ++i) {
        if(c == str[i]) return 1;
    }
    return 0;
}

void removeJunkCharacters(char *str) {
    char removeChars[] = {' ', '\t', '\r', '\n'};
    int k=0;
    for(int i = 0; str[i] != '\0'; ++i) {
        if(!isCharInString(str[i], removeChars))
            str[k++] = str[i];
    }
    str[k] = '\0';
}

int isArithmeticOp(char c) {
  char ops[] = {'+', '-', '*', '/'};
  return isCharInString(c, ops);
}

int isStringEmpty(char *str) {
  return !str || !strcmp(str, "\0") || !strlen(str);
}

int doOpAndPrint (BigInteger *a, BigInteger *b, BigInteger* (*operation)
                                                        (BigInteger*, BigInteger*)) {
  BigInteger *c = (*operation)(a, b);
  if (!c) return -1;
  BigInt_map(c, &printNumber, NULL);
  free(c);
  return 0;
}

int processInput(char *input) {
  if (isStringEmpty(input))
    return -1;
  char op = 'X';
  int start_i = 0, status = 0;
  BigInteger *a, *b;
  for (int i = 0; input[i] != '\0'; ++i) {
    if (isdigit(input[i])) continue;
    else if (isArithmeticOp(input[i])) {
      if (start_i == i) {
        if ((input[i] == '+' || input[i] == '-') && isdigit(input[i+1]))
          continue;
        else return -1;
      } else {
        if (!isArithmeticOp(op)) {
          op = input[i];
          input[i] = '\0';
          a = parseToBigInt(input);
          input[i] = op;
          start_i = i+1;
        }
        else
          return -1;
      }
    } else {
      return -1;
    }
  }
  if (!isArithmeticOp(op)) return -3;
  b = parseToBigInt(input+start_i);
  //BigInt_map(a, &printNumber, NULL);
  //BigInt_map(b, &printNumber, NULL);
  if (!a || !b) return -2;
  switch (op) {
    case '+':
      status = doOpAndPrint(a, b, &addBigInt);
      break;
    case '-':
      status = doOpAndPrint(a, b, &subtractBigInt);
      break;
    case '*':
      status = doOpAndPrint(a, b, &multiplyBigInt);
      break;
    case '/':
      status = doOpAndPrint(a, b, &divideBigInt);
      break;
    default:
      status = -1;
  }
  free(a);
  free(b);
  return status;
}

void calc_loop() {
  char input[1000];
  int status = 0;
  while (1) {
    input[0] = '\0';
    printf("%s", PROMPT);
    scanf("%[^\n]", input);
    getchar();
    removeJunkCharacters(input);
    if (strcmp(input, EXIT_STRING) == 0) return;
    status = processInput(input);
    if (status) printf("Invalid Input Format\n");
  }
}

int main () {
  calc_loop();
  /*BigInteger *a = parseToBigInt("34");
  BigInteger *b = parseToBigInt("-2");
  BigInteger *c = addBigInt(a, b);
  BigInt_map(c, &printNumber, NULL);
  c = addBigInt(a, b);
  a = parseToBigInt("34");
  b = parseToBigInt("-2");
  c = addBigInt(a, b);
  BigInt_map(c, &printNumber, NULL);*/
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <bigInteger.h>
#include <string.h>
#include <utilities.h>

#define PROMPT "Calc> "
#define EXIT_STRING "Exit"

// Prints the given number passed as a string with its sign
void printNumber (char *string, int n, int sign, void *ptr) {
  if (sign == -1) printf("-");
  int i;
  // Skip zeros
  for (i = 0; string[i]=='0' && string[i] != '\0'; ++i);
  if (!string[i]) printf("0\n");
  else printf("%s\n", string+i);
}

// Calls the corresponding BigInt function passed as function pointer
// to perform the arithmetic operation and print the result
int doOpAndPrint (BigInteger *a, BigInteger *b, BigInteger* (*operation)
                                                        (BigInteger*, BigInteger*)) {
  BigInteger *c = (*operation)(a, b);
  if (!c) return -1;
  BigInt_map(c, &printNumber, NULL);
  free(c);
  return 0;
}

// Processes the given input by searching for 2 integers and a binary operation,
// Returns 0 if the operation was successful
int processInput(char *input) {
  if (isStringEmpty(input))
    return -1;
  char op = 'X';
  int start_i = 0, status = 0;
  BigInteger *a, *b;
  for (int i = 0; input[i] != '\0'; ++i) {
    if (status) {
      if (a) free(a);
      return status;
    }
    if (isdigit(input[i])) continue;
    else if (isArithmeticOp(input[i])) {
      if (start_i == i) {
        // If it is the sign of the number instead of operator
        if ((input[i] == '+' || input[i] == '-') && isdigit(input[i+1]))
          continue;
        else status = -1;
      } else {
        // If no operator was parsed till now, store it as op
        if (!isArithmeticOp(op)) {
          op = input[i];
          input[i] = '\0';
          a = parseToBigInt(input);
          input[i] = op;
          start_i = i+1;
        }
        else status = -1;
      }
    } else {
      // If the char is neither a digit nor an arithmetic symbol, input format is illegal
      status = -1;
    }
  }
  if (!isArithmeticOp(op)) return -3;
  b = parseToBigInt(input+start_i);
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
      status = -3;
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
  return 0;
}

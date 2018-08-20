/* -----------------------------------------*
 * Header file for utilities.c              *
 * Check the source file for documentation. *
 *------------------------------------------*/

#ifndef _MY_UTILITIES_
#define _MY_UTILITIES_

int isCharInString(char c, char *str);
void removeJunkCharacters(char *str);
int isArithmeticOp(char c);
int isStringEmpty(char *str);
int sign(char c);
char* uLongToString(unsigned long long n);
int isUpperCase(char c);
int isLowerCase(char c);
int isAlphabet(char c);
int isDigit(char c);
int getDigit(char c);
int encodeAlphabet(char c);
char decodeAlphabet(int n);
int countAlphabets(char *line);
long* getFirstNExponents(int base, int n);

#endif // _MY_UTILITIES_

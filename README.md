## Implementation of Basic Arithmetic Operations for very big integers represented as a string

### To build

```
make
```

### To run
```
./bigCalc
```

### Notes

- Type `Exit` to exit the calculator.
- The input should be of form `operand1 <op> operand2`.
- The operands can have any sign. (+/-).
- Can handle integers as big as `MAX_INT_LENGTH` digits (can be modified in bigInteger.c).
- Addition, subtraction and multiplication are done like regular methods.
- Division is implemented as exponential subtraction (more like how long division works actually) and returns the quotient.

### Footnotes

This was done by me as an assignment while training at [MulticoreWare](https://multicorewareinc.com/).  
Feel free to contribute.

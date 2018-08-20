## Implementation of Basic Arithmetic Operations for very big integers represented as a string

### To build

```
make
```

### To run
```
./bigCalc
```

### Sample I/O
```
$ ./bigCalc 
Calc> 9999999999999524635735*52985256955318
529852569553154812702271599121088730
Calc> 2456574627537577357542662426462-23564626246464373575373
2456574603972951111078288851089
Calc> 1111111111111111+246363727
1111111357474838
Calc> 253252986498628/-7942970
-31883915
Calc> Exit
$ 
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

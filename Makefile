DEPENDENCIES_OBJ=utilities.o bigInteger.o
EXEC=bigCalc
MAIN_FILE=calc.c

%.o: %.c %.h
	gcc -c -o $@ $< -I.

all: $(DEPENDENCIES_OBJ)
	gcc -o $(EXEC) $(MAIN_FILE) $^ -I.

.PHONY: run clean

run:
	echo Starting the Big Calculator....
	./$(EXEC)

clean:
	rm $(DEPENDENCIES_OBJ) $(EXEC)

.SILENT:

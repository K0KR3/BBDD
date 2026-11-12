CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

# recompile if this heder changes
HEADERS = odbc.h customers.h

EXE = menu
OBJ = $(EXE).o odbc.o customers.o
#all : $(EXE)


%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean :
	rm -f *.o core $(EXE)



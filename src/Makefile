# FILES
EXEC := sparc
SRC  := $(wildcard *.cpp)
OBJ  := $(patsubst %.cpp,%.o,$(SRC))

# OPTIONS
CC      := g++ 
CFLAGS  :=-O3 -g -p -Wall -std=c++11 

# RULES
$(EXECDIR)/$(EXEC): $(OBJ)
	$(LINK.o)  -p -fopenmp  -o $@ $^ $(INC) ${LDFLAGS} $(LDLIBS)
%.o: %.cpp
	$(COMPILE.c) $< -fopenmp -o $@ $(INC)

.PHONY: clobber clean 
clobber:
	-$(RM) gmon.out
	-$(RM) $(EXEC)
clean: 
	-rm $(OBJ)


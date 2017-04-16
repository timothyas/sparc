# FILES
EXEC := regression_test
SRC  := $(wildcard *.cpp)
OBJ  := $(patsubst %.cpp,%.o,$(SRC))

# OPTIONS
CC      := g++ 
CFLAGS  :=-O3 -g -p -Wall 
IMP	:= ../../src/graph.cpp 	

# RULES
$(EXEC): $(OBJ)
	$(LINK.o) $(IMP) -p -fopenmp  -o $@ $^ $(INC) ${LDFLAGS} $(LDLIBS)
%.o: %.cpp
	$(COMPILE.c) $< -fopenmp -o $@ $(INC)

.PHONY: clobber clean 
clobber:
	-$(RM) gmon.out
	-$(RM) $(EXEC)
clean: 
	-rm $(OBJ)


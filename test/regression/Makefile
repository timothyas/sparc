# FILES
EXEC := simple_graph
SRC  := $(wildcard *.cpp)
OBJ  := $(patsubst %.cpp,%.o,$(SRC))

# OPTIONS
CC      := g++ 
CFLAGS  :=-O3 -g -p -Wall 
IMP	:= ../../src/graph.cpp 

# RULES
$(EXEC): $(OBJ)
	$(LINK.o) $(IMP) -p -fopenmp  -o $@ $^  $(CFLAGS) $(INC) $(LDFLAGS) $(LDLIBS)
%.o: %.cpp
	$(COMPILE.c)  $< -fopenmp -o $@ $(INC) $(CFLAGS)

.PHONY: clobber clean 
clobber:
	-$(RM) gmon.out
	-$(RM) $(EXEC)
	-$(RM) $(OBJ)
clean: 
	-rm $(OBJ)
check:
	-./$(EXEC)


# FILES
EXEC := regression_tests
SRC  := $(wildcard *.cpp)
OBJ  := $(patsubst %.cpp,%.o,$(SRC))

# OPTIONS
CC      := g++ 
CFLAGS  :=-O3 -g -p -Wall -std=c++11
IMP	:= ../../include/graph.h \
	   ../../include/coarsen.h

# RULES
$(EXEC): $(OBJ)
	$(LINK.o) -p -fopenmp  -o $@ $^  $(CFLAGS) $(INC) $(LDFLAGS) $(LDLIBS)
%.o: %.cpp 
	$(COMPILE.c) $< -fopenmp -o $@ $(INC) $(CFLAGS)
	

.PHONY: clobber clean 
clobber:
	-$(RM) gmon.out
	-$(RM) $(EXEC)
	-$(RM) $(OBJ)
clean: 
	-rm $(OBJ)
	-rm *.o
check:
	-./$(EXEC)


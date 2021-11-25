CC      = glpsol
CCFLAGS = --math

all : solve

solve : src/*
	mkdir -p solutions
	$(CC) $(CCFLAGS) src/ex_1 > solutions/ex_1
	$(CC) $(CCFLAGS) src/ex_2 > solutions/ex_2
	$(CC) $(CCFLAGS) src/ex_3 > solutions/ex_3
	$(CC) $(CCFLAGS) src/ex_4 > solutions/ex_4
	$(CC) $(CCFLAGS) src/ex_5 > solutions/ex_5

.PHONY : clean

clean :
	rm -vfr solutions

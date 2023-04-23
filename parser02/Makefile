all: clean build

clean:
	rm -f *.o

build:
	gcc main.c tinyexpr.c tinyexpr.h -lm -o main 

run:
	./main entrada.txt
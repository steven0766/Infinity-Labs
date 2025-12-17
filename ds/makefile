GD = gcc -ansi -pedantic-errors -Wall -Wextra -g
GC = gcc -ansi -pedantic-errors -Wall -Wextra -DNDBUG -o3

SRCS = foo.c main.c
objects = foo.o main.o
headers = foo.h

objects_release = $(objects:.o=.release.o)

VPATH = src:test:include

.PHONY: default all debug release clean

default: all

debug: fooer.debug.out

all: debug release

release: fooer.out

fooer.debug.out: $(objects)
	$(GD) $(objects) -o $@
	
fooer.out: $(objects_release)
	$(GC) $(objects_release) -o $@
	
%.release.o: %.c $(headers)
	$(GD) -c $< -o $@
	
clean:
	rm -f *.o *.out

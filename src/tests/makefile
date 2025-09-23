#TODO: Enlazar con raylib

CFLAGS=-g
LDFLAGS=-L ../raylib/src/
LDLIBS=-lraylib

test_${TEST}: cpu.o framebuffer.o graphics.o test_${TEST}.o
	gcc $(CFLAGS) $(LDFLAGS) $(LDLIBS) Unity/src/unity.c $^ -o test_${TEST}

cpu.o: ../cpu/cpu.c
	gcc -c $< $(CFLAGS)

framebuffer.o: ../cpu/framebuffer.c
	gcc -c $< $(CFLAGS)

graphics.o: ../graphics.c ../graphics.h
	gcc -c $< $(CFLAGS)

test_${TEST}.o: test_${TEST}.c

clean:
	rm *.o


CFLAGS=-g

cpu.o: ../cpu/cpu.c
	gcc -c ../cpu/cpu.c $(CFLAGS)

framebuffer.o: ../cpu/framebuffer.c
	gcc -c ../cpu/framebuffer.c $(CFLAGS)

test_${TEST}.o: test_${TEST}.c

test_${TEST}: cpu.o framebuffer.o test_${TEST}.o
	gcc Unity/src/unity.c $^ -o test_${TEST}

clean:
	rm *.o


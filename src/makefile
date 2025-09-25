DEBUG=
CFLAGS=-O2 -I raylib/src -I raygui/src
LDFLAGS=-L raylib/src
LDLIBS=-lraylib

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

objects=cpu.o framebuffer.o graphics.o main.o

bocadillo: $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o bocadillo

cpu.o: cpu/cpu.c cpu/cpu.h
	$(CC) $(CFLAGS) $< -c

framebuffer.o: cpu/framebuffer.c cpu/framebuffer.h
	$(CC) $(CFLAGS) $< -c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) $< -c

main.o: main.c
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f *.o


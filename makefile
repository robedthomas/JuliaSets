CC=gcc
CFLAGS=-Wall -std=c99 -O2 -DHAVE_OPENGL -I/usr/local/include
MAC_CFLAGS=-I/opt/local/include
LDFLAGS=-lSDL2 -lSDL2_gfx -lm
MAC_LDFLAGS=-L/opt/local/lib
BUILD_FILES=Project04_01

Project04_01: Project04_01.c JuliaSet.c Drawing.c HelperFunctions.c
	$(CC) $^ -o Project04_01 $(CFLAGS) $(LDFLAGS)

macbuild: Project04_01.c JuliaSet.c Drawing.c HelperFunctions.c
	$(CC) $^ -o Project04_01 $(CFLAGS) $(MAC_CFLAGS) $(LDFLAGS) $(MAC_LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(BUILD_FILES)

.PHONY: gdb
gdb:
	$(CC) Project04_01.c JuliaSet.c Drawing.c HelperFunctions.c -o Project04_01 $(CFLAGS) $(LDFLAGS) -g

.PHONY: test
test: 
	./Project04_01 800 600 4 3 0 0 0.285 0.01 1
	./Project04_01 800 600 2 1.5 0 0 0.285 0.01 1
	./Project04_01 800 600 1 0.75 .45 .22 0.285 0.01 1
	./Project04_01 800 600 4 3 0 0 -0.8 0.156 1
	./Project04_01 800 600
	./Project04_01 800 600 4 3 0 0 0.285 0.01 0
	./Project04_01 0 600 4 3 0 0 0 0 1
	./Project04_01 800 600 four 3 0 0 0 0 1
all:
	gcc -Isrc/Include -Lsrc/lib -o sand sand.c -lmingw32 -lSDL2main -lSDL2

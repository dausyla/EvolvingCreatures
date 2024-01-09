CC=gcc
TARGET=evol_crea

CFLAGS=-Werror -O3
LDFLAGS=-lm -lSDL2

SRC=src/map.c src/creature.c src/tools.c src/main.c src/sdl.c src/natsel.c
OBJS=$(SRC:.c=.o)

all: $(OBJS)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

debug: $(SRC)
	$(CC) $(CFLAGS) -fsanitize=address -g3 $^ -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) -rf $(OBJS) $(TARGET)

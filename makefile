CC = gcc

CFLAGS = -Wall -Wextra -O2 -fopenmp

TARGET = ecosystem

SRC = main.c

NUM_PROCS = 4

ARGS = 5x5

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
	rm -f result*


run:
	./$(TARGET) ecosystem_examples/input$(ARGS)

diff:
	diff output /home/goda/Desktop/fcup/cp/output$(ARGS)


# Phony targets
.PHONY: all clean run diff
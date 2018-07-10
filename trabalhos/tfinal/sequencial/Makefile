# GNU Makefile

CC = gcc 
CFLAGS = -Wall
TARGET = sequential_dotprod

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $^ -o $@ 

all: $(TARGET)

# Dependencias

sequential_dotprod: sequential_dotprod.c

clean:
	rm -f *.o *~ $(TARGET)

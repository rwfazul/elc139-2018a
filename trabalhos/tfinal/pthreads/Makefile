# GNU Makefile

CC = gcc 
CFLAGS = -Wall
LDFLAGS = -pthread
TARGET = pthreads_dotprod

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@ 

all: $(TARGET)

# Dependencias

pthreads_dotprod: pthreads_dotprod.c

clean:
	rm -f *.o *~ $(TARGET)

CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g

all: encode decode error entropy

encode:	encode.o hamming.o bv.o bm.o 
	$(CC) -o encode encode.o hamming.o bv.o bm.o 

decode:	decode.o hamming.o bv.o bm.o 
	$(CC) -o decode decode.o hamming.o bv.o bm.o 

entropy: entropy.o 
	$(CC) $(CFLAGS) -o entropy entropy.o -lm

error:  error.o
	$(CC) $(CFLAGS) -o error error.o

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

entropy.o: entropy.c
	$(CC) $(CFLAGS) -c entropy.c

error.o: error.c
	$(CC) $(CFLAGS) -c error.c

bv.o:	bv.c
	$(CC) $(CFLAGS) -c bv.c

bm.o:	bm.c
	$(CC) $(CFLAGS) -c bm.c

hamming.o:	hamming.c
	$(CC) $(CFLAGS) -c hamming.c

format:	
	clang-format -i -style=file *.[ch]

clean:
	rm -f encode decode entropy error *.o


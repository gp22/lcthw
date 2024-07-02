CC = cc
CFLAGS=-Wall -Wextra -g -O0

all: ex1 ex3 ex7 ex8 ex11 ex15 ex16 ex16ec ex17 ex17ec

clean:
	rm -rf ex1 ex3 ex7 ex8 ex11 ex15 ex16 ex16ec ex17 ex17ec

ex1: ex1.c
	$(CC) $(CFLAGS) ex1.c -o ex1

ex3: ex3.c
	$(CC) $(CFLAGS) ex3.c -o ex3

ex7: ex7.c
	$(CC) $(CFLAGS) ex7.c -o ex7

ex8: ex8.c
	$(CC) $(CFLAGS) ex8.c -o ex8

ex11: ex11.c
	$(CC) $(CFLAGS) ex11.c -o ex11

ex15: ex15.c
	$(CC) $(CFLAGS) ex15.c -o ex15

ex16: ex16.c
	$(CC) $(CFLAGS) ex16.c -o ex16

ex16ec: ex16ec.c
	$(CC) $(CFLAGS) ex16ec.c -o ex16ec

ex17: ex17.c
	$(CC) $(CFLAGS) ex17.c -o ex17

ex17ec: ex17ec.c
	$(CC) $(CFLAGS) ex17ec.c -o ex17ec

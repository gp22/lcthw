CC = cc
CFLAGS=-Wall -Wextra -g -O0

all: ex1 ex3 ex7 ex8 ex11 ex15 ex16 ex16ec ex17 ex17ec ex18 ex19 ex22 ex23 \
	ex24 ex25 ex27_1

clean:
	rm -rf ex1 ex3 ex7 ex8 ex11 ex15 ex16 ex16ec ex17 ex17ec ex18 ex19 \
		ex22.o ex22_main ex23 ex24 ex25 ex27_1

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

ex18: ex18.c
	$(CC) $(CFLAGS) ex18.c -o ex18

ex19: ex19.c
	$(CC) $(CFLAGS) ex19.c -o ex19

ex22: ex22.c
	$(CC) $(CFLAGS) -DNDEBUG -c -o ex22.o ex22.c
	$(CC) $(CFLAGS) -DNDEBUG ex22_main.c ex22.o -o ex22_main

ex23: ex23.c
	$(CC) $(CFLAGS) ex23.c -o ex23

ex24: ex24.c
	$(CC) $(CFLAGS) ex24.c -o ex24

ex25: ex25.c
	$(CC) $(CFLAGS) ex25.c -o ex25

ex27_1: ex27_1.c
	$(CC) $(CFLAGS) ex27_1.c -o ex27_1

logfind: logfind.c
	$(CC) $(CFLAGS) logfind.c -o logfind

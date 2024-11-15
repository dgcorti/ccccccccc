all: mmc

mmc: main.c scanner.h scanner.o
	gcc -c main.c
	gcc -o mmc main.o scanner.o

test: mmc
	./mmc < test.ok.micro
	./mmc < test.error-lex.micro

clean:
	rm -rf *.o mmc

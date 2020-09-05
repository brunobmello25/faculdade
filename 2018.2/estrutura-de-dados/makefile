
program: main.o btree.o linkedlist.o
	gcc -o a.out main.o btree.o linkedlist.o
	rm -rf *.o

main.o: main.c
	gcc -o main.o -c main.c

btree.o: btree.c
	gcc -o btree.o -c btree.c

linkedlist.o: linkedlist.c
	gcc -o linkedlist.o -c linkedlist.c

clean:
	rm -rf *.o
	rm -rf a.out
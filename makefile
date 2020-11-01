file_copy : file_copy.o
	gcc file_copy.o -o file_copy

file_copy.o : file_copy.c
	gcc -c file_copy.c -o file_copy.o

clean :
	rm -f file_copy.o

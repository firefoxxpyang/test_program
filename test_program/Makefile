test_program : main.o
	gcc -g -static -o test_program main.o

main.o : main.c
	gcc -c main.c
	
clean :
	-rm *.o test_program

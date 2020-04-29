main: main.o schedule.o process.o priority.o
	gcc -Wall main.o schedule.o process.o priority.o -o main

schedule.o: schedule.c
	gcc -Wall schedule.c -c

process.o: process.c
	gcc -Wall process.c -c

priority.o: priority.c
	gcc -Wall priority.c -c

clean:
	rm -rf *.o

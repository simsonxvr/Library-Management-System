lms: main.o utils.o issue.o files.o dates.o books.o
	cc main.c utils.c issue.c files.c dates.c books.c -o lms

main.o:main.c 
	cc -c main.c
utils.o:utils.c
	cc -c utils.c
issue.o:issue.c
	cc -c issue.c
files.o:files.c
	cc -c files.c
dates.o:dates.c
	cc -c dates.c
books.o:books.c
	cc -c books.c


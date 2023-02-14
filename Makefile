.PHONY: all
all: PMan inf

PMan: main.c linkedlist.c
	gcc -g main.c linkedlist.c -o PMan

inf: inf.c 
	gcc -g inf.c -o foo


.PHONY: clean
clean:
	-rm -rf PMan foo *.oxs *.exe 


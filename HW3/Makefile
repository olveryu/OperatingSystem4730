.PHONY: all clean

all: processOne processTwo processThree

processOne: processOne.c
	gcc -g -Wall -o processOne processOne.c

processTwo: processTwo.c
	gcc -g -Wall -o processTwo processTwo.c

processThree: processThree.c
	gcc -g -Wall -o processThree processThree.c

clean:
	rm -f processOne
	rm -f processTwo
	rm -f processThree

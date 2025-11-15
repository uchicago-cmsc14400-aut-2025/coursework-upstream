.PHONY: clean

p4: p4machine.h p4machine.c main.c
	clang -Wall -o p4 p4machine.c main.c

debug: p4machine.h p4machine.c main.c
	clang -g -Wall -o p4 p4machine.c main.c

clean:
	rm -rf a.out p4 p4.dSYM

cc=gcc
flags=-Wall -Wextra

sets: main.c
	$(cc) main.c -o sets $(flags)

run:
	$(cc) main.c -o sets $(flags) && rlwrap ./sets

lex: lex.c	
	$(cc) lex.c -o lex $(flags) && rlwrap ./lex

clean:
	rm -rf lex sets hash

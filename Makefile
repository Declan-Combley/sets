cc=gcc
flags=-Wall -Wextra

sets: main.c
	$(cc) main.c -o sets $(flags)

run:
	$(cc) main.c -o sets $(flags) && ./sets

lex: lex.c	
	$(cc) lex.c -o lex $(flags) && rlwrap ./lex

hash: hash.c	
	$(cc) hash.c -o hash $(flags)	

clean:
	rm -rf lex sets hash

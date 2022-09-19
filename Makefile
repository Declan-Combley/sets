cc=gcc
flags=-Wall -Wextra

sets: main.c
	$(cc) main.c -o sets $(flags)

run:
	$(cc) main.c -o sets $(flags) && ./sets

lex: lex.c	
	$(cc) lex.c -o lex $(flags) && ./lex	

clean:
	rm -rf lex set

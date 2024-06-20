all: output 

gpp_lexer: gpp_lexer.l
	flex -o gpp_lexer.c gpp_lexer.l
	gcc gpp_lexer.c -o gpp_lexer
	./gpp_lexer

input:
	flex -o gpp_lexer.c gpp_lexer.l
	gcc gpp_lexer.c -o gpp_lexer
	./gpp_lexer input.txt

output:
	flex -o gpp_lexer.c gpp_lexer.l
	gcc gpp_lexer.c -o gpp_lexer
	./gpp_lexer input.txt > output.txt
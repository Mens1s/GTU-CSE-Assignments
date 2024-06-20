start:
	yacc -d -o gpp_interpreter.c gpp_interpreter.y 
	flex -o gpp_lexer.c gpp_lexer.l 
	gcc gpp_lexer.c gpp_interpreter.c -o gpp_interpreter -ll -w
#./gpp_interpreter
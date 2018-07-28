exe: lexer.l bison.y bison.tab.c lex.yy.c structs.h
	flex lexer.l
	bison bison.y -dv
	gcc bison.tab.c lex.yy.c -o exe
	./exe

.PRECIOUS: exe
.SILENT:

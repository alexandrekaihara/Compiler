all: src/lex.l src/parser.y
	flex src/lex.l
	bison -d src/parser.y
	g++ lex.yy.c parser.tab.c src/ast.cpp -o src/parser

clean:
	rm -f parser parser.o parser.tab.* lex.yy* *.asm *.json

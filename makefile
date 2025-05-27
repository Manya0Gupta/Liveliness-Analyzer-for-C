all: analyzer cfg.png

analyzer: parser.tab.c lex.yy.c symbol_table.o ir_generator.o live_analysis.o cfg.o
	 gcc -o analyzer parser.tab.c lex.yy.c symbol_table.o ir_generator.o live_analysis.o cfg.o -lfl 

parser.tab.c parser.tab.h: parser.y
	 bison -d parser.y

lex.yy.c: lexer.l
	 flex lexer.l

symbol_table.o: symbol_table.c
	 gcc -c symbol_table.c

ir_generator.o: ir_generator.c
	 gcc -c ir_generator.c

live_analysis.o: live_analysis.c
	 gcc -c live_analysis.c

cfg.o: cfg.c
	gcc -c cfg.c

cfg.png: cfg.dot
	dot -Tpng cfg.dot -o cfg.png

clean:
	 rm -f analyzer *.o parser.tab.* lex.yy.c


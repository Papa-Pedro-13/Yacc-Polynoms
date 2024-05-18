#include<stdio.h>
#include "main.tab.c"

extern FILE* f;

int yylex(){
	int c;
	while ((c = getc(f)) == ' ' || c == '\t');
	
	if (c == EOF) {
		return 0;
	}
	if (c >= 'a' && c <= 'z') {
		yylval.sym = c;
		return LETTER_LOW;
	}
	if (isdigit(c)) {
		yylval.num = c - '0';
		return DIGIT;
	}
	return c;
}

int yyerror(const char* str) {
	fprintf(stderr, "error: %s\n", str);
}

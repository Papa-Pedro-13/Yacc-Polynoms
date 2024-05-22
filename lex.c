#include<stdio.h>
#include "main.tab.c"

extern FILE* f;
int line = 1;
int check = 0;
int yylex(){
	int c;
	while ((c = getc(f)) == ' ' || c == '\t');
	//printf("%c", c);
	if (c == '/') {
		char next = getc(f);
		if (next == '/') {
			while ((c = getc(f)) != '\n' && c != EOF);
		}
		else {
			printf("Syntax error: unrecognized symbol in line %d.\n", line + 1);
		}
	}
	if (c == EOF) {
		return 0;
	}
	if (c == '\n') {
		line++;
		check = 0;
	}

	if (c >= 'a' && c <= 'z') {
		yylval.sym = c;
		if (!check) check = c;
		else {
			if (check != c) { yyerror("error - different values"); exit(-1); }
		}
		return LETTER_LOW;
	}
	if (c >= 'A' && c <= 'Z') {
		yylval.sym = c;
		return LETTER_UPPERCASE;
	}
	if (isdigit(c)) {
		yylval.num = c - '0';
		return DIGIT;
	}
	if (c != '?' && c != '+' && c != '-' && c != '=' && c != ' '
		&& c != '*' && c != '^' && c != '(' && c != ')' && c != '\n') {
		printf("Syntax error: unrecognized symbol in line %d.\n",line );
		exit(-1);
	}

	return c;
}

int yyerror(const char* str) {
	fprintf(stderr, "error in line %d: %s\n", line, str);
}

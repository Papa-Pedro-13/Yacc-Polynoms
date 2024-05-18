%{
#include "main.h"
%}

%union {
	int num;
	char sym;
	struct Mono monom;
	struct Polynom poly;
}


%token <sym> LETTER_LOW
%token <num> DIGIT
%token END_OF_LINE
%token POW EQ DOLLAR PLUS MINUS LB RB MUL DIV COMMENT EOC EOL


%type <num> number 
%type <monom> monomial
%type <poly> polynom


%right '='
%left '-' '+'
%left '*'
%left NEG    
%right '^'  


%%
file: 
	| line 
	| file line
;

line: '\n'
	| polynom '\n' {polynomPrint($1);}
;
	
polynom: monomial {$$ = polynomInit($1);}
	| polynom '+' polynom {$$ = polynomSum($1,$3)}
	| polynom '-' polynom {$$ = polynomSub($1,$3)}
	| polynom '*' polynom {$$ = polynomMul($1,$3)}
	| polynom '^' number {$$ = polynomPow($1,$3)}
	| '(' polynom ')' { $$ = $2; }
;

	
monomial: number LETTER_LOW '^' number{$$.coefficient = $1;$$.deg=$4;$$.name=$2}
	| number LETTER_LOW{$$.coefficient = $1;$$.deg=1;$$.name=$2}	
	| LETTER_LOW '^' number{$$.coefficient = 1;$$.deg=$3;$$.name=$1}
	| number {$$.coefficient = $1;$$.deg=0;$$.name=0}
	| LETTER_LOW {$$.coefficient = 1;$$.deg=1;$$.name=$1}

;
	

number: DIGIT{$$=$1;} 
	| number DIGIT { $$ = $1*10  + $2; }
	| number '^' number {$$ = pow($1,$3);} 
;

%%
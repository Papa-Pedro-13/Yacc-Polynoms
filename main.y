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
%token <sym> LETTER_UPPERCASE
%token <num> DIGIT
%token END_OF_LINE


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
	| LETTER_UPPERCASE '=' polynom '\n' { polynomVarInit($1, $3);}
    	| '?' LETTER_UPPERCASE '\n' {polynomVarPrint($2);}
	| error '\n' { yyerrok; printf("Error in this line.\n");  }
;
	
polynom: monomial {$$ = polynomInit($1);}
	| LETTER_UPPERCASE { $$ = putPolynom($1);}
/*	| polynom '(' polynom ')' polynom { $$ = polynomMul($1, $5);} */
	| polynom '+' polynom {$$ = polynomSum($1,$3)}
	| polynom '-' polynom {$$ = polynomSub($1,$3)}
	| polynom '*' polynom {$$ = polynomMul($1,$3)}
	//| '(' polynom ')' polynom { $$ = polynomMul($2, $4);} 
	//| polynom '(' polynom ')' { $$ = polynomMul($1, $3);} 
        | '-' polynom %prec NEG { $$ = unaryMinus($2); }
	| polynom '^' number {$$ = polynomPow($1,$3)}
	| polynom '^' polynom {$$ = polynomPowPolynom($1,$3)}
	| polynom polynom %prec '*' { $$ = polynomMul($1, $2);} 
	| '(' polynom ')' { $$ = $2; }
;

	
monomial: number LETTER_LOW '^' number{$$.coefficient = $1;$$.deg=$4;$$.name=$2}
	| number LETTER_LOW{$$.coefficient = $1;$$.deg=1;$$.name=$2}	
	| LETTER_LOW number {$$.coefficient = $2;$$.deg=1;$$.name=$1}	
	| LETTER_LOW '^' number{$$.coefficient = 1;$$.deg=$3;$$.name=$1}
	| LETTER_LOW {$$.coefficient = 1;$$.deg=1;$$.name=$1}
	| number {$$.coefficient = $1; $$.deg=0; $$.name=0}
;


number: DIGIT{$$=$1;} 
	| number DIGIT { $$ = $1*10  + $2; }
	| number '^' number {$$ = pow($1,$3);} 
;

%%
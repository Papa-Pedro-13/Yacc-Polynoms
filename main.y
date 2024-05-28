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
%left NEG    
%left '*'
%right '^'  


%%
file: 
	| line 
	| file line
;

line: '\n'
	| polynom '\n' {polynomPrint($1);}
	| LETTER_UPPERCASE '=' polynom '\n' { polynomVarInit($1, $3);}
	| LETTER_LOW '=' polynom '\n' { yyerror("syntax error - can't assign poly to poly"); }
	| error '\n' { yyerrok;  }
;
	
polynom: monomial {$$ = polynomInit($1);}
	| LETTER_UPPERCASE { $$ = putPolynom($1);}
/*	| polynom '(' polynom ')' polynom { $$ = polynomMul($1, $5);} */
	| polynom '+' polynom {$$ = polynomSum($1,$3);if($$.error==1) YYERROR;}
	| polynom '-' polynom {$$ = polynomSub($1,$3);if($$.error==1) YYERROR;}
	| polynom '*' polynom {$$ = polynomMul($1,$3);if($$.error==1) YYERROR;}
        | '-' polynom %prec NEG { $$	 = unaryMinus($2); }
	| polynom '^' polynom {$$ = polynomPowPolynom($1,$3);if($$.error==1) YYERROR;}
	| polynom polynom %prec '*' { $$ = polynomMul($1, $2);} 
	| '(' polynom ')' { $$ = $2; }
;

	
monomial: LETTER_LOW {$$.coefficient = 1;$$.deg=1;$$.name=$1}
	| number {$$.coefficient = $1; $$.deg=0; $$.name=0}	
;


number: DIGIT{$$=$1;} 
	| number DIGIT { $$ = $1*10  + $2; }
;

%%
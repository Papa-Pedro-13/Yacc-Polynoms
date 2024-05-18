#pragma once
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_LEN 100
#define COEF 1
#define DEG 0
struct Mono{
	int coefficient;
	int deg;
	char name;
};

struct Polynom {
	int koef[MAX_LEN][2];
	char name;
};


//void addVariableToList(char*, struct Polynom);
//struct Polynom* getVariable(char*);
//struct Mono* setMono(int, char*, int);
struct Polynom polynomInit(struct Mono);
//struct Polynom* addMononomToPolynom(struct Polynom, struct Mono);
//struct Node* deleteNode(struct Polynom*, struct Node*);
//struct Polynom* deleteSimilarSummands(struct Polynom);
struct Polynom polynomSum(struct Polynom, struct Polynom);
struct Polynom polynomSub(struct Polynom, struct Polynom);
struct Polynom polynomMul(struct Polynom, struct Polynom);
struct Polynom polynomPow(struct Polynom a, int number);
//struct Polynom* unaryMinus(struct Polynom*);
void errPrint(const char*, const char*);
void monoPrint(struct Mono*);
void polynomPrint(struct Polynom);

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"	

struct Polynom polynomInit(struct Mono mono) {
    struct Polynom poly;
    poly.name = mono.name;
    for (int i = 0; i < MAX_LEN; i++) {
        poly.koef[i][0] = -1;
        poly.koef[i][1] = 0;
    }
    poly.koef[0][0] = mono.deg;
    poly.koef[0][1] = mono.coefficient;
    return poly;
}
int indexOfKey(struct Polynom a, int key) {
    
}
struct Polynom polynomSum(struct Polynom a, struct Polynom b) {
    //Sum all in A and in B
    for (int i = 0; i < MAX_LEN; i++) {
        for (int j = 0; j < MAX_LEN; j++) {
            if (b.koef[j][DEG] == a.koef[i][DEG]) {
                a.koef[i][COEF] += b.koef[j][COEF];
            }
        }
    }
    //Sum what only in B
    for (int i = 0; i < MAX_LEN; i++) {
        bool wasInA = false;
        for (int j = 0; j < MAX_LEN; j++) {
            if (b.koef[i][DEG] == a.koef[j][DEG]) {
                wasInA = true;
            }
        }
        //Add to A monomial
        if (!wasInA) {
            int index = 0;
            while (a.koef[index][DEG] != -1) {
                index++;
            }
            a.koef[index][DEG] = b.koef[i][DEG];
            a.koef[index][COEF] = b.koef[i][COEF];
        }
    }
    return a;
};
struct Polynom polynomSub(struct Polynom a, struct Polynom b) {
    //Sub all in A and in B
    for (int i = 0; i < MAX_LEN; i++) {
        for (int j = 0; j < MAX_LEN; j++) {
            if (b.koef[j][DEG] == a.koef[i][DEG]) {
                a.koef[i][COEF] -= b.koef[j][COEF];
            }
        }
    }
    //Sub what only in B
    for (int i = 0; i < MAX_LEN; i++) {
        bool wasInA = false;
        for (int j = 0; j < MAX_LEN; j++) {
            if (b.koef[i][DEG] == a.koef[j][DEG]) {
                wasInA = true;
            }
        }
        //Sub to A monomial
        if (!wasInA) {
            int index = 0;
            while (a.koef[index][0] != -1) {
                index++;
            }
            a.koef[index][DEG] = b.koef[i][DEG];
            a.koef[index][COEF] -= b.koef[i][COEF];
        }
    }
    return a;
}; 
int hasDeg(struct Polynom a,int deg) {
    for (int j = 0; j < MAX_LEN; j++) {
        if (a.koef[j][DEG] == deg) {
            return j;
        }
    }
    return -1;
}
struct Polynom polynomMul(struct Polynom a, struct Polynom b) {
    struct Mono empty;
    empty.deg = 0;
    empty.coefficient = 0;
    empty.name = a.name;
    struct Polynom newPolynom = polynomInit(empty);
    for (int i = 0; i < MAX_LEN; i++) {
        for (int j = 0; j < MAX_LEN; j++) {
            int index = hasDeg(newPolynom, a.koef[i][DEG] + b.koef[j][DEG]);
            if (index == -1) {
                index = 0;
                while (newPolynom.koef[index][0] != -1) {
                    index++;
                }
                newPolynom.koef[index][DEG] = a.koef[i][DEG] + b.koef[j][DEG];
            }
            newPolynom.koef[index][COEF] += a.koef[i][COEF] * b.koef[j][COEF];
        }
    }
    if (a.name == 0)
        newPolynom.name = b.name;

    return newPolynom;

};
struct Polynom polynomPow(struct Polynom a,int number) {
    struct Polynom newPolynom;
    if (number == 0) {
        struct Mono empty;
        empty.deg = 0;
        empty.coefficient = 1;
        empty.name = a.name;

        newPolynom = polynomInit(empty);
        return newPolynom;
    }
    newPolynom = a;
    for (int i = 0; i < number - 1; i++) {
        newPolynom = polynomMul(newPolynom, a);
    }
    return newPolynom;
}
void polynomPrint(struct Polynom a) {
    bool firstPrint = true;
    for (int i = MAX_LEN-1; i >= 0; i--) {
        //Ненулевой одночлен
        if (a.koef[i][COEF] != 0) {
            //Не первый и положительный одночлен - выводим плюс
            if (!firstPrint && a.koef[i][COEF]>0) {
                printf("+");
            }
            //Одночлен с ненулевой степенью
            if (a.koef[i][DEG] != 0) {
                if (a.koef[i][COEF] != 1) {
                    //Случай: -1x
                    if (a.koef[i][COEF] == -1) {
                        printf("-");
                    }
                    else {
                        printf("%d", a.koef[i][COEF]);
                    }
                }
                //Выводим букву (x)
                printf("%c", a.name);
                //Степень
                if (a.koef[i][DEG] > 1) {
                    printf("^%d", a.koef[i][DEG]);
                }            
            }
            //Число
            else {
                printf("%d", a.koef[i][COEF]);
            }
            firstPrint = false;
        }
    }
    printf("\n");
};
FILE* f;

int main() {
	f = fopen("example.txt","r");
	yyparse();
	fclose(f);
}

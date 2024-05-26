#define _CRT_SECURE_NO_WARNINGS
#include "main.h"	

struct Polynom polynomInit(struct Mono mono) {
    struct Polynom poly;
    poly.name = mono.name;
    for (int i = 0; i < MAX_LEN; i++) {
        poly.koef[i][DEG] = -1;
        poly.koef[i][COEF] = 0;
    }
    poly.koef[0][DEG] = mono.deg;
    poly.koef[0][COEF] = mono.coefficient;
    poly.error = 0;
    return poly;
}
struct Polynom polynomSum(struct Polynom a, struct Polynom b) {
    //check for multiple values
    if (a.name!=0 && b.name!=0 && a.name != b.name) {
        yyerror("different values");
        a.error = 1; //we change only function-local a.error state to 1, in global meaning it's still a.error == 0 for a
        return a;    //so it's ok to change a there

    }
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
    if (a.name == 0)
        a.name = b.name;
    return a;
};
struct Polynom polynomSub(struct Polynom a, struct Polynom b) {
    //check for multiple values
    if (a.name != 0 && b.name != 0 && a.name != b.name) {
        yyerror("different values");
        a.error = 1; //we change only function-local a.error state to 1, in global meaning it's still a.error == 0 for a
        return a;    //so it's ok to change a there

    }
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
    if (a.name == 0)
        a.name = b.name;
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
    //check for multiple values
    if (a.name != 0 && b.name != 0 && a.name != b.name) {
        yyerror("different values");
        a.error = 1; //we change only function-local a.error state to 1, in global meaning it's still a.error == 0 for a
        return a;    //so it's ok to change a there

    }
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
    if (number < 0) {
        yyerror("negative power");
        newPolynom.error = 1;
        return newPolynom;
    }
    if (number == 0) {
        bool isZero = true;
        for (int i = 0; i < MAX_LEN; i++) {
            if (a.koef[i][COEF] != 0) {
                isZero = false;
            }
        }
        if (isZero) {
            yyerror("undefined 0^0");
            newPolynom.error = 1;
            return newPolynom;
        }
    }
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
        for (int j = 0; j < MAX_LEN; j++) {
            if (newPolynom.koef[j][COEF] ==  0 && newPolynom.koef[j][DEG] != 0) {
                newPolynom.koef[j][DEG] = -1;
                newPolynom.koef[j][COEF] = 0;
            }
        }
    }
    return newPolynom;
}
//void polynomPrint(struct Polynom a) {
//    bool firstPrint = true;
//    bool isZero = true;
//    for (int i = MAX_LEN-1; i >= 0; i--) {
//        //��������� ��������
//        if (a.koef[i][COEF] != 0) {
//            isZero = false;
//            //�� ������ � ������������� �������� - ������� ����
//            if (!firstPrint && a.koef[i][COEF]>0) {
//                printf("+");
//            }
//            //�������� � ��������� ��������
//            if (a.koef[i][DEG] != 0) {
//                if (a.koef[i][COEF] != 1) {
//                    //������: -1x
//                    if (a.koef[i][COEF] == -1) {
//                        printf("-");
//                    }
//                    else {
//                        printf("%d", a.koef[i][COEF]);
//                    }
//                }
//                //������� ����� (x)
//                printf("%c", a.name);
//                //�������
//                if (a.koef[i][DEG] > 1) {
//                    printf("^%d", a.koef[i][DEG]);
//                }            
//            }
//            //�����
//            else {
//                printf("%d", a.koef[i][COEF]);
//            }
//            firstPrint = false;
//        }
//    }
//    if (isZero) printf("0");
//    printf("\n");
//};
int isAllPrinted(struct Polynom a) {
    int resultDeg = -1;
    int index = -1;

    //check if there is not a null degree
    for (int i = MAX_LEN - 1; i >= 0; i--) {
        if (a.koef[i][COEF] != 0) {
            if (a.koef[i][DEG] > resultDeg) { resultDeg = a.koef[i][DEG]; index = i; }
        }
    }

    return index;
}
void polynomPrint(struct Polynom a) {
    bool isZero = true;
    bool firstPrint = true;
    int indexOfMax = isAllPrinted(a);

    //until all components printed
    while (indexOfMax != -1) {
        isZero = false;
        
        //***PRINT STARTS HERE***
        //not first and positive
        if (!firstPrint && a.koef[indexOfMax][COEF] > 0) {
            printf("+");
        }

        //printing COEF
        if (a.koef[indexOfMax][COEF] != 1) {
            if (a.koef[indexOfMax][COEF] == -1) printf("-");
            else printf("%d", a.koef[indexOfMax][COEF]); 
        }

        //printing letter
        if (a.koef[indexOfMax][DEG] != 0) printf("%c", a.name);

        //printing degree
        if (a.koef[indexOfMax][DEG] > 1) printf("^%d", a.koef[indexOfMax][DEG]);

        firstPrint = false;
        // ***END OF PRINT***

        a.koef[indexOfMax][DEG] = -1;
        a.koef[indexOfMax][COEF] = 0;
        indexOfMax = isAllPrinted(a);
    }

    if (isZero) printf("0");
    printf("\n");
}
struct Polynom unaryMinus(struct Polynom a) {
    struct Mono empty;
    empty.deg = 0;
    empty.coefficient = 0;
    empty.name = a.name;
    struct Polynom reverse = polynomInit(empty);
    for (int i = 0; i < MAX_LEN; i++) {
        reverse.koef[i][DEG] = a.koef[i][DEG];
        reverse.koef[i][COEF] = -1 * a.koef[i][COEF];
    }
    return reverse;

}
struct Polynom polynomPowPolynom(struct Polynom a, struct Polynom b) {
    struct Polynom res;
    for (int i = 0; i < MAX_LEN; i++) {
        if (b.koef[i][DEG] > 0) {
            yyerror("pow polynom in polynom");
            //return;
            res.error = 1;
            return res;
        }
    }
    int index = hasDeg(b, 0);
    
    if (index !=-1) {
        
        res = polynomPow(a, b.koef[index][COEF]);
    }
    else {
        res = polynomPow(a, 0);
    }
    return res;
}
struct Polynom putPolynom(char name) {
    if (varsArray[name - 'A'].name != name) {
        printf("Variable %c is not found\n", name);
        struct Polynom  a = { .error = 1 };
        return a;
    }
    return varsArray[name - 'A'].polynom;
}

void polynomVarPrint(char name) {
    printf("polynomVarPrint\n");
}
void polynomVarInit(char name, struct Polynom a) {
    varsArray[name - 'A'].name = name;
    varsArray[name - 'A'].polynom = a;
    /*if (varsArray[name - 'A'].name = ) {
        varsArray[].name = name;
        g_varray.arr[g_varray.count++].mempoly = polynom_copy(p);
    }
    else {
        g_varray.arr[i].mempoly = polynom_copy(p);
    }*/
}


FILE* f;

int main(int argc, char*argv[]) {
	f = fopen(argv[1],"r");
	yyparse();
	fclose(f);
    //������ ���� ������������ �����
    //������� �� ���������� ���������� A=x B=y A=A-B ������ ���� �������
    
}

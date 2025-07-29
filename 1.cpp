#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    float coef;
    int exp;
} Term;

typedef struct {
    Term* termArray;
    int capacity;
    int terms;
} Polynomial;

// 建立 Polynomial
Polynomial Polynomial_create(int initialCapacity) {
    if (initialCapacity <= 0) {
        fprintf(stderr, "Invalid capacity!\n");
        exit(1);
    }

    Polynomial p;
    p.capacity = initialCapacity;
    p.terms = 0;
    p.termArray = (Term*)malloc(sizeof(Term) * initialCapacity);
    return p;
}

// 解構 Polynomial
void Polynomial_destroy(Polynomial* p) {
    free(p->termArray);
    p->termArray = NULL;
    p->capacity = 0;
    p->terms = 0;
}

// 複製 Polynomial
void Polynomial_copy(Polynomial* dest, const Polynomial* src) {
    dest->capacity = src->capacity;
    dest->terms = src->terms;
    dest->termArray = (Term*)malloc(sizeof(Term) * src->capacity);
    memcpy(dest->termArray, src->termArray, sizeof(Term) * src->terms);
}

// 新增一項
void Polynomial_newTerm(Polynomial* p, float coef, int exp) {
    if (coef == 0) return;
    if (p->terms >= p->capacity) {
        p->capacity *= 2;
        Term* newArray = (Term*)malloc(sizeof(Term) * p->capacity);
        memcpy(newArray, p->termArray, sizeof(Term) * p->terms);
        free(p->termArray);
        p->termArray = newArray;
    }
    p->termArray[p->terms].coef = coef;
    p->termArray[p->terms].exp = exp;
    p->terms++;
}

// 多項式加法
Polynomial Polynomial_add(const Polynomial* a, const Polynomial* b) {
    Polynomial result = Polynomial_create(4);
    int i = 0, j = 0;

    while (i < a->terms && j < b->terms) {
        if (a->termArray[i].exp == b->termArray[j].exp) {
            float sum = a->termArray[i].coef + b->termArray[j].coef;
            if (sum != 0)
                Polynomial_newTerm(&result, sum, a->termArray[i].exp);
            i++; j++;
        } else if (a->termArray[i].exp > b->termArray[j].exp) {
            Polynomial_newTerm(&result, a->termArray[i].coef, a->termArray[i].exp);
            i++;
        } else {
            Polynomial_newTerm(&result, b->termArray[j].coef, b->termArray[j].exp);
            j++;
        }
    }

    while (i < a->terms) Polynomial_newTerm(&result, a->termArray[i].coef, a->termArray[i++].exp);
    while (j < b->terms) Polynomial_newTerm(&result, b->termArray[j].coef, b->termArray[j++].exp);

    return result;
}

// 多項式乘法
Polynomial Polynomial_multiply(const Polynomial* a, const Polynomial* b) {
    Polynomial result = Polynomial_create(4);

    for (int i = 0; i < a->terms; ++i) {
        for (int j = 0; j < b->terms; ++j) {
            float prodCoef = a->termArray[i].coef * b->termArray[j].coef;
            int prodExp = a->termArray[i].exp + b->termArray[j].exp;

            if (prodCoef != 0) {
                int found = 0;
                for (int k = 0; k < result.terms; ++k) {
                    if (result.termArray[k].exp == prodExp) {
                        result.termArray[k].coef += prodCoef;
                        found = 1;
                        break;
                    }
                }
                if (!found) Polynomial_newTerm(&result, prodCoef, prodExp);
            }
        }
    }

    return result;
}

// 多項式求值
float Polynomial_eval(const Polynomial* p, float x) {
    float result = 0;
    for (int i = 0; i < p->terms; ++i) {
        result += p->termArray[i].coef * powf(x, p->termArray[i].exp);
    }
    return result;
}

// 輸入多項式
void Polynomial_input(Polynomial* p) {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        float coef;
        int exp;
        scanf("%f%d", &coef, &exp);
        Polynomial_newTerm(p, coef, exp);
    }
}

// 輸出多項式
void Polynomial_print(const Polynomial* p) {
    for (int i = 0; i < p->terms; ++i) {
        if (i > 0) printf(" + ");
        printf("%.2fx^%d", p->termArray[i].coef, p->termArray[i].exp);
    }
    printf("\n");
}

// 主程式
int main() {
    Polynomial A = Polynomial_create(4);
    Polynomial B = Polynomial_create(4);

    printf("Polynomial A=");
    Polynomial_input(&A);
    printf("Polynomial B=");
    Polynomial_input(&B);

    Polynomial sum = Polynomial_add(&A, &B);
    Polynomial prod = Polynomial_multiply(&A, &B);

    printf("A + B = ");
    Polynomial_print(&sum);
    printf("A * B = ");
    Polynomial_print(&prod);

    float x;
    printf("x=");
    scanf("%f", &x);

    printf("A(%.2f) = %.2f\n", x, Polynomial_eval(&A, x));
    printf("B(%.2f) = %.2f\n", x, Polynomial_eval(&B, x));
    printf("A + B(%.2f) = %.2f\n", x, Polynomial_eval(&sum, x));
    printf("A * B(%.2f) = %.2f\n", x, Polynomial_eval(&prod, x));

    return 0;
}

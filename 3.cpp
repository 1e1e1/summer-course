#include <iostream>
#include <cmath>

class Term {
public:
    int coef;
    int exp;
    Term* link;
};

class Polynomial {
private:
    Term* header;
    Term* available;
    Term* allocateTerm() {
        if (available) {
            Term* temp = available;
            available = available->link;
            return temp;
        }
        return new Term;
    }
    void deallocateTerm(Term* term) {
        term->link = available;
        available = term;
    }
public:
    Polynomial() {
        header = new Term;
        header->link = header;
        available = NULL;
    }
    ~Polynomial() {
        Term* current = header->link;
        while (current != header) {
            Term* temp = current;
            current = current->link;
            deallocateTerm(temp);
        }
        delete header;
    }
    friend std::istream& operator>>(std::istream& is, Polynomial& p) {
        p.~Polynomial();
        p.header = new Term;
        p.header->link = p.header;
        p.available = NULL;
        int n;
        if (!(is >> n)) return is;
        if (n == 0) return is;
        Term* current = p.header;
        for (int i = 0; i < n; ++i) {
            int c, e;
            if (!(is >> c >> e)) {
                std::cerr << "Error: Insufficient data for " << n << " terms." << std::endl;
                return is;
            }
            Term* newTerm = p.allocateTerm();
            newTerm->coef = c;
            newTerm->exp = e;
            newTerm->link = p.header;
            current->link = newTerm;
            current = newTerm;
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        Term* current = p.header->link;
        bool first = true;
        bool hasTerms = false;
        while (current != p.header) {
            if (current->coef == 0) {
                current = current->link;
                continue;
            }
            hasTerms = true;
            if (!first && current->coef > 0) os << " + ";
            if (current->coef < 0) os << " - ";
            int absCoef = std::abs(current->coef);
            if (absCoef != 1 || current->exp == 0) os << absCoef;
            if (current->exp > 0) os << "x";
            if (current->exp > 1) os << "^" << current->exp;
            first = false;
            current = current->link;
        }
        if (!hasTerms) os << "0";
        return os;
    }
    Polynomial(const Polynomial& a) {
        header = new Term;
        header->link = header;
        available = NULL;
        Term* src = a.header->link;
        Term* dest = header;
        while (src != a.header) {
            Term* newTerm = allocateTerm();
            newTerm->coef = src->coef;
            newTerm->exp = src->exp;
            dest->link = newTerm;
            dest = newTerm;
            src = src->link;
        }
        dest->link = header;
    }
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            this->~Polynomial();
            new (this) Polynomial(a);
        }
        return *this;
    }
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Term* resultTail = result.header;
        Term* aPtr = header->link;
        while (aPtr != header) {
            Term* newTerm = result.allocateTerm();
            newTerm->coef = aPtr->coef;
            newTerm->exp = aPtr->exp;
            newTerm->link = result.header;
            resultTail->link = newTerm;
            resultTail = newTerm;
            aPtr = aPtr->link;
        }
        Term* bPtr = b.header->link;
        while (bPtr != b.header) {
            Term* current = result.header->link;
            Term* prev = result.header;
            bool merged = false;
            while (current != result.header) {
                if (current->exp == bPtr->exp) {
                    current->coef += bPtr->coef;
                    if (current->coef == 0) {
                        prev->link = current->link;
                        result.deallocateTerm(current);
                        current = prev->link;
                    }
                    merged = true;
                    break;
                }
                prev = current;
                current = current->link;
            }
            if (!merged && bPtr->coef != 0) {
                Term* newTerm = result.allocateTerm();
                newTerm->coef = bPtr->coef;
                newTerm->exp = bPtr->exp;
                newTerm->link = result.header;
                resultTail->link = newTerm;
                resultTail = newTerm;
            }
            bPtr = bPtr->link;
        }
        return result;
    }
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Term* resultTail = result.header;
        Term* aPtr = header->link;
        while (aPtr != header) {
            Term* newTerm = result.allocateTerm();
            newTerm->coef = aPtr->coef;
            newTerm->exp = aPtr->exp;
            newTerm->link = result.header;
            resultTail->link = newTerm;
            resultTail = newTerm;
            aPtr = aPtr->link;
        }
        Term* bPtr = b.header->link;
        while (bPtr != b.header) {
            Term* current = result.header->link;
            Term* prev = result.header;
            bool merged = false;
            while (current != result.header) {
                if (current->exp == bPtr->exp) {
                    current->coef -= bPtr->coef;
                    if (current->coef == 0) {
                        prev->link = current->link;
                        result.deallocateTerm(current);
                        current = prev->link;
                    }
                    merged = true;
                    break;
                }
                prev = current;
                current = current->link;
            }
            if (!merged && bPtr->coef != 0) {
                Term* newTerm = result.allocateTerm();
                newTerm->coef = -bPtr->coef;
                newTerm->exp = bPtr->exp;
                newTerm->link = result.header;
                resultTail->link = newTerm;
                resultTail = newTerm;
            }
            bPtr = bPtr->link;
        }
        return result;
    }
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Term* aPtr = header->link;
        while (aPtr != header) {
            Term* bPtr = b.header->link;
            while (bPtr != b.header) {
                Term* newTerm = result.allocateTerm();
                newTerm->coef = aPtr->coef * bPtr->coef;
                newTerm->exp = aPtr->exp + bPtr->exp;
                Term* current = result.header;
                Term* next = current->link;
                while (next != result.header) {
                    if (next->exp == newTerm->exp) {
                        next->coef += newTerm->coef;
                        result.deallocateTerm(newTerm);
                        if (next->coef == 0) {
                            current->link = next->link;
                            result.deallocateTerm(next);
                        }
                        newTerm = NULL;
                        break;
                    }
                    current = next;
                    next = next->link;
                }
                if (newTerm && newTerm->coef != 0) {
                    newTerm->link = result.header;
                    current->link = newTerm;
                } else if (newTerm) {
                    result.deallocateTerm(newTerm);
                }
                bPtr = bPtr->link;
            }
            aPtr = aPtr->link;
        }
        return result;
    }
    float Evaluate(float x) const {
        float result = 0;
        Term* current = header->link;
        while (current != header) {
            result += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;
    std::cin >> p1;
    std::cout << "p1 = " << p1 << std::endl;
    std::cin >> p2;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p1 + p2 = " << p1 + p2 << std::endl;
    std::cout << "p1 - p2 = " << p1 - p2 << std::endl;
    std::cout << "p1 * p2 = " << p1 * p2 << std::endl;
    std::cout << "p1(2.0) = " << p1.Evaluate(2.0) << std::endl;
    return 0;
}

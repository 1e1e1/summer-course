#include <iostream>
#include <cstdlib>
using namespace std;

int A(int m, int n) {
    const int MAX_STACK = 10000;
    int stack[MAX_STACK];
    int top = 0;

    stack[top++] = m;

    while (top > 0) {
        m = stack[--top];
        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        } else {
            stack[top++] = m - 1;
            stack[top++] = m;    
            n = n - 1;
        }

        if (top >= MAX_STACK) {
            cout << "溢位！" << endl;
            exit(1);
        }
    }

    return n;
}

int main() {
    int m, n;
    cout << "請輸入m: ";
    cin >> m;
    cout << "請輸入n: ";
    cin >> n;

    int result = A(m, n);
    cout << "A(" << m << ", " << n << ") = " << result << endl;

    return 0;
}

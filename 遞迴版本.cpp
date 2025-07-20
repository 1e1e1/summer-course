#include <iostream>
#include <cstdlib>
using namespace std;

int A(int m,int n){
	while (m!=0) {
		if (n == 0) {
            m=m-1;
            n=1;
        } else {
			n=A(m,n-1);
			m=m-1;
		}
	}
	return n+1;
}



int main() {
    int m, n;
    cout << "請輸入m:";
    cin >> m ;
    cout << "請輸入n:";
    cin >> n;

    int result = A(m, n);
    cout << "A(" << m << ", " << n << ") = " << result << endl;

    return 0;
}

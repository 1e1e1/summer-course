#include <iostream>
#include <string>
using namespace std;

void powerSet(string str, int index = 0, string curr = "") {
    int n = str.length();

    if (index == n) {
        cout << "(";
        for (size_t i = 0; i < curr.length(); ++i) {
            cout << curr[i];
            if (i != curr.length() - 1) cout << ",";
        }
        cout << ")" << endl;
        return;
    }

    powerSet(str, index + 1, curr + str[index]); 
    powerSet(str, index + 1, curr);               
}

int main() {
    string str = "abc";
    cout << "請輸入: ";
    getline(cin,str);
    
    powerSet(str);
    return 0;
}

#include <bits/stdc++.h>

using namespace std;

int main(){
    vector<int> a = vector<int>();
    vector<int> b = {1, 2, 3, 4};
    for (int i = 1; i < 5; i++){
        a.push_back(i);
    }
    cout << ((a == b) ? "CORRECT" : "WRONG") << endl;
}
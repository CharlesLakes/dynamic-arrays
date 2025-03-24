#include "../../src/brodnik_vector_cpp/brodnik_vector.hpp"
#include <iostream>

using namespace std;

int main(){
    int n;
    cin >> n;

    brodnik_vector<int> vec(n);
    for (int i = 0; i < n; i++)
        cin >> vec[i];

    int q; cin >> q;
    while(q--){
        int op;
        cin >> op;
        if(op){
            int value; 
            cin >> value;
            vec.push_back(value);
        }else vec.pop_back();
    }

    for (int i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;

    return 0;
}

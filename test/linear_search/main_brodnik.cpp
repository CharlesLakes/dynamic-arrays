#include "../../src/brodnik_vector_cpp/brodnik_vector.hpp"
#include "../debug.h"
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
        int num;
        cin >> num;
        bool flag = false;
        for(int i = 0; i < n; i++)
            flag |= num == vec[i];
        PRINT_INT(flag);
    }

    PRINT_CHECK();
    return 0;
}

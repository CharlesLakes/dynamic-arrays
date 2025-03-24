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
        int num;
        cin >> num;
        bool flag = false;
    
        int l = 0, r = n - 1;
        while(l <= r){
            int mid = (l+r)/2;
            int value = vec[mid];
            if(value == num){
                flag = 1;
                break;
            }

            if(valud < num) l = mid + 1;
            else r = mid - 1;
        }

        cout << flag << " ";
    }

    return 0;
}

#include <bits/stdc++.h>
#include "../../src/brodnik_vector_cpp/brodnik_vector.hpp"
#include "heap.hpp"

using namespace std;

int main(){
    int operations;
    cin >> operations;

    heap<brodnik_vector<int>,int> pq;

    while(operations--){
        int op;
        cin >> op;
        if(op){
            int value;
            cin >> value;
            pq.push({value});
        }else{
            pq.pop();
        }

        cout << pq.top() << endl;
    }

    return 0;
}
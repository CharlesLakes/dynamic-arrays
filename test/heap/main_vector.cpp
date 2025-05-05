#include <bits/stdc++.h>
#include "heap.hpp"

using namespace std;

int main(){
    int operations;
    cin >> operations;

    heap<vector<int>,int> pq;

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
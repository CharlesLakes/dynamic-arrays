#include <bits/stdc++.h>
#include "heap.hpp"

using namespace std;

int main(){
    heap<vector<int>,int> pq;

    pq.push(10);
    pq.push(-1);
    pq.push(11);
    cout << pq.top() << endl;

    return 0;
}
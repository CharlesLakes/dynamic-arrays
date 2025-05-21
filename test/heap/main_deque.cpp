#include <bits/stdc++.h>

using namespace std;

int main(){
    int operations;
    cin >> operations;

    priority_queue<int,deque<int>> pq;

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
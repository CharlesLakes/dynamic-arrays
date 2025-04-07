#include "../debug.h"
#include<bits/stdc++.h>

using namespace std;

int main(){
    stack<int> st,rev;
    int n;
    cin >> n;
    while(n--){
        int num; cin >> num;
        st.push(num);
    }

    int q;
    cin >> q;
    while(q--){
        int o; cin >> o;
        if(o){
            int value; cin >> value;
            st.push(value);
        }else{
            st.pop();
        }
    }

    while(!st.empty()) rev.push(st.top()), st.pop();

    while(!rev.empty()){
        PRINT_INT(rev.top());
        rev.pop();
    }


    PRINT_CHECK();
    return 0;
}
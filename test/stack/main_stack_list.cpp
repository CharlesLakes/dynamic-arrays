#include "../debug.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    stack<int, list<int>> st;
    
    int number_of_element;
    cin >> number_of_element;
    
    while(number_of_element--){
        int number; 
        cin >> number;
        
        st.push(number);
    }

    int queries;
    cin >> queries;
    while(queries--){
        int operation, value; 
        cin >> operation; 
        cin >> value;
        if(operation){
            st.push(value);
        }else{
            st.pop();
        }
    }

    while(!st.empty()){
        PRINT_INT(st.top());
        st.pop();
    }

    PRINT_CHECK();
    return 0;
}
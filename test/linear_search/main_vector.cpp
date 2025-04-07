#include "../debug.h"
#include<bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int &num: nums) cin >> num;
    int q; cin >> q;
    while(q--){
        int num;
        cin >> num;
        bool flag = false;
        for(int i = 0; i < n; i++)
            flag |= num == nums[i];
        PRINT_INT(flag);
    }

    PRINT_CHECK();
    return 0;
}
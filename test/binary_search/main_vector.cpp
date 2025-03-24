#include <bits/stdc++.h>

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
        int l = 0, r = n - 1;
        while(l <= r){
            int mid = (l+r)/2;
            int value = nums[mid];
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
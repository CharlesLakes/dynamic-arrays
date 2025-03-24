#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int &num: nums) cin >> num;
    sort(nums.begin(),nums.end());
    for(int num: nums) cout << num << " "; cout << endl;

    return 0;
}
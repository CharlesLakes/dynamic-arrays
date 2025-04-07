#include "../debug.h"
#include<bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int &num: nums) cin >> num;
    sort(nums.begin(),nums.end());
    for(int num: nums) PRINT_INT(2*num); PRINT_END();

    PRINT_CHECK();
    return 0;
}
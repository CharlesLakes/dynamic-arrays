#include <bits/stdc++.h>
//#include "../../src/brodnik_vector/brodnik_vector.hpp"

using namespace std;


int main(){
    int n; cin >> n;
    vector<int> nums(n);
    for(int &a: nums) cin >> a;
    sort(nums.begin(),nums.end());

    for(int a: nums) cout << a << " "; cout << endl;
    return 0;
}
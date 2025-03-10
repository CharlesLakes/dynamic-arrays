#include <bits/stdc++.h>
#include "../../src/brodnik_vector/brodnik_vector.hpp"

using namespace std;

void merge(brodnik_vector<int> &vec, int l, int r){
    brodnik_vector<int> aux;
    int mid = (l + r)/2;

    int i = l, j = mid + 1;

    while(i <= mid && j <= r){
        if(vec[i] <= vec[j]) aux.push_back(vec[i]), i++;
        else aux.push_back(vec[j]), j++;
    }

    while(i <= mid) aux.push_back(vec[i]), i++;
    while(j <= r) aux.push_back(vec[j]), j++;

    for(int i = l; i <= r; i++)
        vec[i] = aux[i - l];
}

void merge_sort(brodnik_vector<int> &vec, int l, int r){
    if(l >= r) return;
    int mid = (l + r)/2;
    merge_sort(vec,l,mid);
    merge_sort(vec,mid + 1,r);
    merge(vec,l,r);
}

int main(){
    int n; cin >> n;
    brodnik_vector<int> vec(n);
    for(int i = 0; i < n; i++)
        cin >> vec[i];

    merge_sort(vec,0,n-1);

    for(int i = 0; i < n; i++)
        cout << vec[i] << " "; cout << endl;
    return 0;
}
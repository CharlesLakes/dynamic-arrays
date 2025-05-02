#include "../debug.h"
#include <bits/stdc++.h>

using namespace std;

void merge(deque<int> &vec, int l, int r) {
  deque<int> aux;
  int mid = (l + r) / 2;
  int i = l, j = mid + 1;

  // Merge two sorted subarrays
  while (i <= mid && j <= r) {
    if (vec[i] <= vec[j])
      aux.push_back(vec[i++]);
    else
      aux.push_back(vec[j++]);
  }

  // Add remaining elements from left and right subarrays
  while (i <= mid)
    aux.push_back(vec[i++]);
  while (j <= r)
    aux.push_back(vec[j++]);

  // Copy sorted elements back to the original array
  for (int k = l; k <= r; k++)
    vec[k] = aux[k - l];
}

void merge_sort(deque<int> &vec, int l, int r) {
  if (l >= r)
    return;
  int mid = (l + r) / 2;

  merge_sort(vec, l, mid);     // Sort left half
  merge_sort(vec, mid + 1, r); // Sort right half
  merge(vec, l, r);            // Merge both halves
}

int main(){
  int n;
  cin >> n;
  deque<int> nums(n);
  for(int &num: nums) cin >> num;
  merge_sort(nums,0,n - 1);
  for(int num: nums) PRINT_INT(num); PRINT_END();

  PRINT_CHECK();
  return 0;
}
#include "../../src/brodnik_vector_c/brodnik_vector.c"
#include "../debug.h"
#include <stdio.h>

void merge(brodnik_vector *vec, int l, int r) {
  brodnik_vector aux;
  init(&aux);
  int mid = (l + r) / 2;
  int i = l, j = mid + 1;

  // Merge two sorted subarrays
  while (i <= mid && j <= r) {
    if ((*locate(vec, i)) <= (*locate(vec, j)))
      push_back(&aux, *locate(vec, i++));
    else
      push_back(&aux, *locate(vec, j++));
  }

  // Add remaining elements from left and right subarrays
  while (i <= mid)
    push_back(&aux, *locate(vec, i++));
  while (j <= r)
    push_back(&aux, *locate(vec, j++));

  // Copy sorted elements back to the original array
  for (int k = l; k <= r; k++) {
    int *ptr_value = locate(vec, k);
    *ptr_value = *locate(&aux, k - l);
  }

  delete (&aux);
}

void merge_sort(brodnik_vector *vec, int l, int r) {
  if (l >= r)
    return;
  int mid = (l + r) / 2;

  merge_sort(vec, l, mid);     // Sort left half
  merge_sort(vec, mid + 1, r); // Sort right half
  merge(vec, l, r);            // Merge both halves
}

int main() {
  brodnik_vector vec;

  init(&vec);

  int n;
  scanf("%d", &n);

  for (int i = 0; i < n; i++) {
    int x;
    scanf("%d", &x);
    push_back(&vec, x);
  }

  merge_sort(&vec, 0, n - 1);

  for (int i = 0; i < n; i++) {
    PRINT_INT( *locate(&vec, i));
  }

  delete (&vec);

  PRINT_CHECK();
  return 0;
}

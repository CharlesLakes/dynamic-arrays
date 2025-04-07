#include "../../src/brodnik_vector_c/brodnik_vector.c"
#include "../debug.h"
#include <stdio.h>

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

  int q;
  scanf("%d", &q);
  while(q--){
    int num;
    scanf("%d", &num);
    int flag = 0;
    for(int i = 0; i < n; i++)
      flag |= num == *locate(&vec,i);
    PRINT_INT( flag);
  }

  delete (&vec);

  PRINT_CHECK();
    return 0;
}

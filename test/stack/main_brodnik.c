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
    int op;
    scanf("%d", &op);
    int value;
    scanf("%d", &value);
    if(op){
        push_back(&vec, value);
    }else{
        pop_back(&vec);
    }
  }

  while(vec.n_size > 0){
    PRINT_INT(*locate(&vec, vec.n_size - 1));
    pop_back(&vec);
  }  
  delete (&vec);

  PRINT_CHECK();
  return 0;
}

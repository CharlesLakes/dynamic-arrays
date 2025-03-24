#include "../../src/brodnik_vector_c/brodnik_vector.c"
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
    if(op){
        int value;
        scanf("%d", &value);
        push_back(&vec, value);
    }else{
        pop_back(&vec);
    }
  }

  for (int i = 0; i < vec.n_size; i++) {
    printf("%d ", *locate(&vec, i));
  }

  delete (&vec);

  return 0;
}

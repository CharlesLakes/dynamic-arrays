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

        int l = 0, r = n - 1;
        while(l <= r){
            int mid = (l+r)/2;
            int value = *locate(&vec, mid);
            if(value == num){
                flag = 1;
                break;
            }

            if(value < num) l = mid + 1;
            else r = mid - 1;
        }

        PRINT_INT( flag);
    }

    delete (&vec);

    PRINT_CHECK();
    return 0;
}

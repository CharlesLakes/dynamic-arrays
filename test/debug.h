#ifndef CHECK_SO
    static int CHECK_SO = 0;
    static int CHECK_CNT = 0;
#endif

#ifdef DEBUG_SO
    #define PRINT_INT(value)    (printf("%d ", (value)))
    #define PRINT_END()         printf("\n")
    #define PRINT_CHECK()       ((void)0)
#else
    #define PRINT_INT(value)    (CHECK_SO ^= (value^CHECK_CNT), CHECK_CNT++)
    #define PRINT_END()         ((void)0)
    #define PRINT_CHECK()       (printf("%d\n", (CHECK_SO)))
#endif
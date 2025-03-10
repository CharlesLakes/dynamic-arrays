#ifndef BRODNIK_VECTOR_HPP
#define BRODNIK_VECTOR_HPP

typedef struct {
    int n_size;
    int size_value;

    // super block
    int sb_index; // s - 1
    int sb_size;
    int sb_max_size;

    // data block
    int db_index; // d - 1
    int db_size;
    int db_max_size;
    
    // index block
    int ib_size;
    int ib_max_size;
    int **index_block;
} brodnik_vector;

void init(brodnik_vector* this);
int* locate(brodnik_vector *this,int i);
void push_back(brodnik_vector *this, int value);
void pop_back(brodnik_vector *this);
void delete(brodnik_vector *this);

#endif
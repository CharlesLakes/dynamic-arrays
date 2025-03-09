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
    void **index_block;
} brodnik_vector;

void init(brodnik_vector* this, int size_value);
void* locate(brodnik_vector *this,int i);
void push_back(brodnik_vector *this, void *value);
void pop_back(brodnik_vector *this);

#define INIT(type,this) init(this,sizeof(type))

#define GET_REF(type,this,position) (type *)locate(this,position)

#define GET_VALUE(type,this,position) *(GET_REF(type,this,position))

#define PUSH_BACK(type,this,value) { \
    type temporal_value_vector = value; \
    push_back(this,(void *) &temporal_value_vector); \
}

#define POP_BACK(type,this) pop_back(this)

#endif
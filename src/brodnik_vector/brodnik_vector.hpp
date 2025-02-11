template <class T>
class brodnik_vector{
    private:
        int n_size;

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
        T **index_block;
        
        void grow();
        void shrink();
        T locate(int i);

    public:
        brodnik_vector();
        ~brodnik_vector();
        
        
        void push_back(int value);
        
        void pop_back();

        T& operator[](int index);

};
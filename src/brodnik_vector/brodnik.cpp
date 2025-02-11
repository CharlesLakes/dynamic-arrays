#define fast_log2(x) (31 - __builtin_clz(int(x)))

void brodnik_vector::grow(){
    // If the last nonempty data block DB_{d-1} is full
    if(this->db_size == this->db_max_size){
        // If the last superblock SB_{s-i} is full
        if(this->sb_size == this->sb_max_size){
            this->sb_index++; // Increment s
            if(this->sb_index % 2)
                this->db_max_size *= 2; // s even
            else
                this->sb_max_size *= 2; // s odd
            this->sb_size = 0;
        }

        //If there are no empty data blocks
        if(this->db_index + 2 == this->ib_size){
            if(this->ib_size == this->ib_max_size){
                // TODO: utilizar realloc y abstraerlo
                T **new_index_block = new *T[this->ib_size + 1];
                for(int i = 0; i < this->ib_size; i++)
                    new_index_block[i] = this->index_block[i];
                delete[] this->index_block;
                this->index_block = new_index_block;
                this->ib_max_size++;
            }
            this->index_block[this->ib_size] = new *T[this->db_max_size];
            this->ib_size++;
        }

        this->db_index++;
        this->sb_size++;
        this->db_size = 0;
    }

    this->n_size++;
    this->db_size++;
}

void brodnik_vector::shrink(){
    this->n_size--;
    this->db_size--;
    if(!this->db_size){
        if(this->db_index + 2 == this->ib_size){
            delete[] this->index_block[this->db_index + 1];
        }

        // TODO: utilizar realloc y abstraerlo
        if(this->ib_size*4 <= this->ib_max_size){
            this->ib_max_size = this->ib_size;
            T **new_index_block = new *T[this->ib_size];
            for(int i = 0; i < this->ib_size; i++)
                new_index_block[i] = this->index_block[i];
            delete[] this->index_block;
            this->index_block = new_index_block;
        }
        this->db_index--;
        this->sb_size--;
        if(!this->sb_size){
            this->sb_index--;
            if(sb_index % 2)
                this->db_max_size /= 2;
            else this->sb_max_size /= 2;
            this->sb_size = this->sb_max_size;
        }
    }
}

void brodnik_vector::locate(int i){
    int r = i + 1;
    int k = fast_log2(i+1);
    int b = (r >> ((k+1)/2)) - (1 << (k/2));
    int e = (r & ((1 << ((k+1)/2)) - 1));
    int p = (1 << ((k+1)/2)) + (1 << ((k)/2)) - 2;
    return this->index_block[p+b][e];
}
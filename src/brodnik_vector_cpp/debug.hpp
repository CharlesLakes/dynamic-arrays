#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG
    #define DEBUG_MSG(msg) std::cerr << "[DEBUG] " << msg << std::endl;
    #define DEBUG_INT_VAR(var) std::cerr << "[DEBUG] " << #var << " = " << var << std::endl;
    #define DEBUG_THIS(stage) \
        std::cerr << "[DEBUG] Stage: " << stage << "\n"; \
        DEBUG_INT_VAR(this->n_size); \
        DEBUG_INT_VAR(this->sb_index); \
        DEBUG_INT_VAR(this->sb_size); \
        DEBUG_INT_VAR(this->sb_max_size); \
        DEBUG_INT_VAR(this->db_index); \
        DEBUG_INT_VAR(this->db_size); \
        DEBUG_INT_VAR(this->db_max_size); \
        DEBUG_INT_VAR(this->ib_size); \
        DEBUG_INT_VAR(this->ib_max_size); \
        std::cerr << "\n\n";
#else
    #define DEBUG_MSG(msg)
    #define DEBUG_INT_VAR(var)
    #define DEBUG_THIS(stage)
#endif

#endif
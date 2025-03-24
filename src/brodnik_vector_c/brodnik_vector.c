#include "brodnik_vector.h"
#include <stdlib.h>
#define fast_log2(x) (31 - __builtin_clz(x))

void init(brodnik_vector *this) {
  this->size_value = sizeof(int);

  this->db_size = 0;
  this->sb_size = 1;

  this->db_max_size = this->sb_max_size = 1;

  this->db_index = this->sb_index = 0;

  this->ib_size = 1;
  this->ib_max_size = 1;

  this->index_block = (int **)malloc(sizeof(int *));
  this->index_block[0] = (int *)malloc(sizeof(int));

  this->n_size = 0;
}

void grow(brodnik_vector *this) {
  if (this->db_size == this->db_max_size) {

    if (this->sb_size == this->sb_max_size) {
      this->sb_index++; // Increment s
      if (this->sb_index % 2)
        this->db_max_size *= 2; // s even
      else
        this->sb_max_size *= 2; // s odd
      this->sb_size = 0;
    }

    if (this->db_index + 2 > this->ib_size) {
      if (this->ib_size == this->ib_max_size) {

        // TODO: utilizar realloc y abstraerlo
        int **new_index_block = (int **)malloc(
            (this->ib_size + 1) * sizeof(int *)); // new T*[this->ib_size + 1];
        for (int i = 0; i < this->ib_size; i++)
          new_index_block[i] = this->index_block[i];

        free(this->index_block);

        this->index_block = NULL;
        this->index_block = new_index_block;

        this->ib_max_size++;
      }
      this->index_block[this->ib_size] = (int *)malloc(
          this->db_max_size * this->size_value); // new T[this->db_max_size];
      this->ib_size++;
    }

    this->db_index++;
    this->db_size = 0;

    this->sb_size++;
  }

  this->db_size++;
  this->n_size++;
}

void shrink(brodnik_vector *this) {
  this->n_size--;
  this->db_size--;

  if (!this->db_size) {
    if (this->db_index + 2 == this->ib_size) {
      free(this->index_block[this->db_index + 1]);
      // delete[] this->index_block[this->db_index + 1];
      this->index_block[this->db_index + 1] = NULL;
      this->ib_size--;
    }

    // TODO: utilizar realloc y abstraerlo
    if (this->ib_size * 4 <= this->ib_max_size) {
      this->ib_max_size = this->ib_size;
      int **new_index_block = (int **)malloc(this->ib_size * sizeof(int *));
      // T **new_index_block = new T*[this->ib_size];
      for (int i = 0; i < this->ib_size; i++)
        new_index_block[i] = this->index_block[i];

      free(this->index_block);
      this->index_block = NULL;
      this->index_block = new_index_block;
    }

    this->db_index--;
    this->sb_size--;

    if (!this->sb_size) {
      this->sb_index--;
      if (this->sb_index % 2)
        this->db_max_size /= 2;
      else
        this->sb_max_size /= 2;
      this->sb_size = this->sb_max_size;
    }
    this->db_size = this->db_max_size;
  }
}

int *locate(brodnik_vector *this, int i) {
  int r = i + 1;
  int k = fast_log2(i + 1);
  int b = (r >> ((k + 1) / 2)) - (1 << (k / 2));
  int e = (r & ((1 << ((k + 1) / 2)) - 1));
  int p = (1 << ((k + 1) / 2)) + (1 << ((k) / 2)) - 2;

  void *block = this->index_block[p + b];
  void *value = block + e * (this->size_value);
  return value;
}

void push_back(brodnik_vector *this, int value) {
  grow(this);

  int *current_cell = locate(this, this->n_size - 1);

  *current_cell = value;
}

void pop_back(brodnik_vector *this) { shrink(this); }

void delete (brodnik_vector *this) {
  for (int i = 0; i < this->ib_size; i++) {
    if (this->index_block[i] == NULL)
      continue;
    free(this->index_block[i]);
  }
  if (this->index_block != NULL)
    free(this->index_block);
}
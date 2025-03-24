#ifndef BRODNIK_VECTOR_HPP
#define BRODNIK_VECTOR_HPP
#include <iostream>
#include "debug.hpp"

template <class T> class brodnik_vector {
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

  void init();

  void grow();
  void shrink();
  T &locate(int i);

public:
  brodnik_vector();
  brodnik_vector(int n);

  ~brodnik_vector();

  void push_back(T value);

  void pop_back();

  int size();

  T &operator[](int index);
};

#define fast_log2(x) (31 - __builtin_clz(x))

template <class T> brodnik_vector<T>::brodnik_vector() { init(); }

template <class T> brodnik_vector<T>::brodnik_vector(int n) {
  init();
  while (n--)
    this->grow();
}

template <class T> int brodnik_vector<T>::size(){
  return this->n_size;
}

template <class T> void brodnik_vector<T>::init() {
  DEBUG_THIS("START-INIT");
  this->db_size = 0;
  this->sb_size = 1;

  this->db_max_size = this->sb_max_size = 1;

  this->db_index = this->sb_index = 0;

  this->ib_size = 1;
  this->ib_max_size = 1;

  this->index_block = new T *[1];
  this->index_block[0] = new T[1];

  this->n_size = 0;

  DEBUG_THIS("END-INIT");
}

template <class T> brodnik_vector<T>::~brodnik_vector() {
  for (int i = 0; i < this->ib_size; i++) {
    if (this->index_block[i] == nullptr)
      continue;
    delete[] this->index_block[i];
  }
  if (this->index_block != nullptr)
    delete[] this->index_block;
}

template <class T> void brodnik_vector<T>::grow() {
  DEBUG_THIS("START-GROW");
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
        T **new_index_block = new T *[this->ib_size + 1];
        for (int i = 0; i < this->ib_size; i++)
          new_index_block[i] = this->index_block[i];

        delete[] this->index_block;
        this->index_block = nullptr;
        this->index_block = new_index_block;

        this->ib_max_size++;
      }
      this->index_block[this->ib_size] = new T[this->db_max_size];
      this->ib_size++;
    }

    this->db_index++;
    this->db_size = 0;

    this->sb_size++;
  }

  this->db_size++;
  this->n_size++;

  DEBUG_THIS("END-GROW");
}

template <class T> void brodnik_vector<T>::shrink() {
  DEBUG_THIS("START-SHIRNK");
  // Decrement n
  this->n_size--;

  // Decrement the number of elements occupying the last nonempty data block
  this->db_size--;

  // Check if last used datablock is empty
  if (!this->db_size) {
    // If there is another empty data block. Deallocate it.
    if (this->db_index + 2 == this->ib_size) {
      delete[] this->index_block[this->db_index + 1];
      this->index_block[this->db_index + 1] = nullptr;
      this->ib_size--;
    }

    // If the index block is a quarter full. Reallocate it to half its size.
    // TODO: utilizar realloc y abstraerlo
    if (this->ib_size * 4 <= this->ib_max_size) {
      this->ib_max_size = this->ib_size;
      T **new_index_block = new T *[this->ib_size];
      for (int i = 0; i < this->ib_size; i++)
        new_index_block[i] = this->index_block[i];

      delete[] this->index_block;
      this->index_block = new_index_block;
    }

    // Decrement index of last used datablock
    this->db_index--;
    // Decrement the number of datablocks used in current superblock
    this->sb_size--;

    // if the last superblock is empty.
    if (!this->sb_size) {
      // Decrement the superblock index
      this->sb_index--;
      if (this->sb_index % 2 == 0)
        this->db_max_size /= 2;
      else
        this->sb_max_size /= 2;
      this->sb_size = this->sb_max_size;
    }
    // Set the ocuppancy of the last superblock to full
    this->db_size = this->db_max_size;
  }
  DEBUG_THIS("END-SHIRNK");
}

template <class T> T &brodnik_vector<T>::locate(int i) {
  int r = i + 1;
  int k = fast_log2(i + 1);
  int b = (r >> ((k + 1) / 2)) - (1 << (k / 2));
  int e = (r & ((1 << ((k + 1) / 2)) - 1));
  int p = (1 << ((k + 1) / 2)) + (1 << ((k) / 2)) - 2;
  return this->index_block[p + b][e];
}

template <class T> void brodnik_vector<T>::push_back(T value) {
  this->grow();

  T &current_cell = this->locate(this->n_size - 1);

  current_cell = value;
}

template <class T> void brodnik_vector<T>::pop_back() { this->shrink(); }

template <class T> T &brodnik_vector<T>::operator[](int index) {
  return this->locate(index);
}

#endif
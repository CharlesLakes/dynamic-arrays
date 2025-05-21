#ifndef BRODNIK_VECTOR_HPP
#define BRODNIK_VECTOR_HPP

#include <iostream>
#include "debug.hpp"

//
// This code implements a space-efficient dynamic array (vector) based on the structure
// proposed by Brodnik. It is maintained by CharlesLakes.
//

/**
 * @brief A space-efficient dynamic array implementation based on Brodnik's structure.
 *
 * This class provides a dynamic array with efficient space utilization by
 * using a hierarchical structure of blocks of varying sizes.
 *
 * @tparam T The type of elements stored in the vector.
 */
template <class T> class brodnik_vector {
private:
  unsigned int n_size; // Current number of elements in the vector

  // Superblock management
  int sb_index; // s - 1
  int sb_size;
  int sb_max_size;

  // Data block
  int db_index; // d - 1
  int db_size;
  int db_max_size;

  // Index block
  // The index block is a dynamic array of pointers to data blocks.
  int ib_size;
  int ib_max_size;
  T **index_block;

  /**
   * @brief Initializes the brodnik_vector.
   *
   * Sets up the initial state of the vector, including the superblock,
   * datablock, and index block.
   */
  void init();

  /**
   * @brief Grows the vector by adding a new element.
   *
   * Manages the allocation of data blocks and superblocks to accommodate the new element.
   */
  void grow();

  /**
   * @brief Shrinks the vector by removing the last element.
   *
   * Deallocates data blocks and superblocks if they become empty.
   */
  void shrink();

  /**
   * @brief Locates the data block and offset for a given index.
   * 
   * @param i Index to locate.
   * @return Reference to the element at the index.
   */
  T &locate(int i);

  /**
   * @brief Const version of locate function.
   *
   * @param i Index to locate.
   * @return Const reference to the element at the index.
   */
  T &locate(int i) const;

public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = unsigned int;
  using difference_type = std::ptrdiff_t;

  /**
   * @brief An iterator for the brodnik_vector.
   *
   * Provides random access iteration capabilities over the elements in the vector.
   */
  class iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    iterator(brodnik_vector* vec, size_type pos) : vec_(vec), pos_(pos) {}

    reference operator*() const { return (*vec_)[pos_]; }
    pointer operator->() const { return &(*vec_)[pos_]; }

    iterator& operator++() { ++pos_; return *this; }
    iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

    iterator& operator--() { --pos_; return *this; }
    iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

    iterator operator+(difference_type n) const { return iterator(vec_, pos_ + n); }
    iterator operator-(difference_type n) const { return iterator(vec_, pos_ - n); }
    difference_type operator-(const iterator& other) const { return pos_ - other.pos_; }

    bool operator==(const iterator& other) const { return pos_ == other.pos_; }
    bool operator!=(const iterator& other) const { return pos_ != other.pos_; }
    bool operator<(const iterator& other) const { return pos_ < other.pos_; }
    bool operator>(const iterator& other) const { return pos_ > other.pos_; }
    bool operator<=(const iterator& other) const { return pos_ <= other.pos_; }
    bool operator>=(const iterator& other) const { return pos_ >= other.pos_; }

  private:
    brodnik_vector* vec_;
    size_type pos_;
  };

  /**
   * @brief Returns an iterator to the beginning of the vector.
   */
  iterator begin() { return iterator(this, 0); }

  /**
   * @brief Returns an iterator to the end of the vector.
   */
  iterator end() { return iterator(this, size()); }

  /**
   * @brief Constructs an empty brodnik_vector.
   */
  brodnik_vector();

  /**
   * @brief Constructs a brodnik_vector with a specified initial size.
   * @param n Initial number of elements to allocate.
   */
  brodnik_vector(int n);

  /**
   * @brief Destructor. Frees all allocated memory.
   */
  ~brodnik_vector();

  /**
   * @brief Adds an element to the end of the vector.
   * @param value The value to be added.
   */
  void push_back(T value);

  /**
   * @brief Removes the last element of the vector.
   */
  void pop_back();

  /**
   * @brief Returns the number of elements in the vector.
   */
  size_type size() const;

  /**
   * @brief Checks whether the vector is empty.
   */
  bool empty() const;

  /**
   * @brief Returns a reference to the last element.
   */
  reference back();

  /**
   * @brief Returns a reference to the first element.
   */
  reference front();

  /**
   * @brief Returns a const reference to the last element.
   */
  const_reference back() const;

  /**
   * @brief Returns a const reference to the first element.
   */
  const_reference front() const;

  /**
   * @brief Access element at specific index.
   * @param index Index of the element.
   * @return Reference to the element at the given index.
   */
  T &operator[](int index);
};

// A fast approximation of log base 2. Used in the locate function.
#define fast_log2(x) (31 - __builtin_clz(x))

// Default constructor
template <class T> brodnik_vector<T>::brodnik_vector() { init(); }

// Constructor with size
template <class T> brodnik_vector<T>::brodnik_vector(int n) {
  init();
  while (n--)
    this->grow();
}

// Return size
template <class T> typename brodnik_vector<T>::size_type brodnik_vector<T>::size() const {
  return this->n_size;
}

// Initialization
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

// Destructor
template <class T> brodnik_vector<T>::~brodnik_vector() {
  for (int i = 0; i < this->ib_size; i++) {
    if (this->index_block[i] == nullptr)
      continue;
    delete[] this->index_block[i];
  }
  if (this->index_block != nullptr)
    delete[] this->index_block;
}

// Grow
template <class T> void brodnik_vector<T>::grow() {
  DEBUG_THIS("START-GROW");
  if (this->db_size == this->db_max_size) {
    if (this->sb_size == this->sb_max_size) {
      this->sb_index++;
      if (this->sb_index % 2)
        this->db_max_size *= 2;
      else
        this->sb_max_size *= 2;
      this->sb_size = 0;
    }

    if (this->db_index + 2 > this->ib_size) {
      if (this->ib_size == this->ib_max_size) {
        T **new_index_block = new T *[2 * this->ib_size];
        for (int i = 0; i < this->ib_size; i++)
          new_index_block[i] = this->index_block[i];
        delete[] this->index_block;
        this->index_block = new_index_block;
        this->ib_max_size *= 2;
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

// Shrink
template <class T> void brodnik_vector<T>::shrink() {
  DEBUG_THIS("START-SHIRNK");
  this->n_size--;
  this->db_size--;

  if (!this->db_size) {
    if (this->db_index + 2 == this->ib_size) {
      delete[] this->index_block[this->db_index + 1];
      this->index_block[this->db_index + 1] = nullptr;
      this->ib_size--;
    }

    if (this->ib_size * 4 <= this->ib_max_size) {
      this->ib_max_size = this->ib_size;
      T **new_index_block = new T *[this->ib_size];
      for (int i = 0; i < this->ib_size; i++)
        new_index_block[i] = this->index_block[i];
      delete[] this->index_block;
      this->index_block = new_index_block;
    }

    this->db_index--;
    this->sb_size--;

    if (!this->sb_size) {
      this->sb_index--;
      if (this->sb_index % 2 == 0)
        this->db_max_size /= 2;
      else
        this->sb_max_size /= 2;
      this->sb_size = this->sb_max_size;
    }

    this->db_size = this->db_max_size;
  }

  DEBUG_THIS("END-SHIRNK");
}

// Locate
template <class T> T &brodnik_vector<T>::locate(int i) {
  int r = i + 1;
  int k = fast_log2(i + 1);
  int b = (r >> ((k + 1) / 2)) - (1 << (k / 2));
  int e = (r & ((1 << ((k + 1) / 2)) - 1));
  int p = (1 << ((k + 1) / 2)) + (1 << ((k) / 2)) - 2;
  return this->index_block[p + b][e];
}

template <class T> T &brodnik_vector<T>::locate(int i) const {
  int r = i + 1;
  int k = fast_log2(i + 1);
  int b = (r >> ((k + 1) / 2)) - (1 << (k / 2));
  int e = (r & ((1 << ((k + 1) / 2)) - 1));
  int p = (1 << ((k + 1) / 2)) + (1 << ((k) / 2)) - 2;
  return this->index_block[p + b][e];
}

// Push back
template <class T> void brodnik_vector<T>::push_back(T value) {
  this->grow();
  T &current_cell = this->locate(this->n_size - 1);
  current_cell = value;
}

// Pop back
template <class T> void brodnik_vector<T>::pop_back() {
  this->shrink();
}

// Check if empty
template <class T> bool brodnik_vector<T>::empty() const {
  return n_size == 0;
}

// Get last element
template <class T> T &brodnik_vector<T>::back() {
  return this->locate(n_size - 1);
}

// Get first element
template <class T> T &brodnik_vector<T>::front() {
  return this->locate(0);
}

// Get const last element
template <class T> const T &brodnik_vector<T>::back() const {
  return this->locate(n_size - 1);
}

// Get const first element
template <class T> const T &brodnik_vector<T>::front() const {
  return this->locate(0);
}

// Indexing operator
template <class T> T &brodnik_vector<T>::operator[](int index) {
  return this->locate(index);
}

#endif

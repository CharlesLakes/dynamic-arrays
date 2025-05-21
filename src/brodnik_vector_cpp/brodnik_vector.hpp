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

  // data block
  int db_index; // d - 1
  int db_size;
  int db_max_size;

  // index block
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
   * Deallocates data blocks and superblocks if they become empty.
   */

  void shrink();
  
  T &locate(int i);

  T &locate(int i) const;

  public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = int;
  using difference_type = std::ptrdiff_t;

  /**
   * @brief An iterator for the brodnik_vector.
   *
   * Provides random access iteration capabilities over the elements in the vector.
   */
  class iterator {
    public:
      // Iterator traits for compatibility with standard library algorithms
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
   * @return An iterator pointing to the first element.
   */
  iterator begin() { return iterator(this, 0); }

  /**
   * @brief Returns an iterator to the end of the vector.
   * @return An iterator pointing to the theoretical element after the last element.
   */
  iterator end() { return iterator(this, size()); }

  /**
   * @brief Constructs an empty brodnik_vector.
   */
  brodnik_vector();

  /**
   * @brief Constructs a brodnik_vector with a specified initial size.
   * @param n The initial number of elements to allocate.
   */
  brodnik_vector(int n);

  /**
   * @brief Destroys the brodnik_vector.
   *
   * Deallocates all the memory used by the data blocks and the index block.
   */

  ~brodnik_vector();

  void push_back(T value);

  void pop_back();

  int size();

  bool empty() const;

  T& back();

  T& front();

  T& back() const;

  T& front() const;

  T &operator[](int index);
};

// A fast approximation of log base 2. Used in the locate function.
#define fast_log2(x) (31 - __builtin_clz(x))

// Default constructor: Initializes an empty vector.
template <class T> brodnik_vector<T>::brodnik_vector() { init(); }

// Constructor with initial size: Initializes a vector with 'n' elements, growing as needed.
template <class T> brodnik_vector<T>::brodnik_vector(int n) {
  init();
  while (n--)
    this->grow();
}

// Returns the current number of elements in the vector.
template <class T> int brodnik_vector<T>::size(){
  return this->n_size;
}

/**
 * @brief Initializes the brodnik_vector.
 *
 * Sets up the initial state of the vector, including the superblock,
 * datablock, and index block.
 */
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

/**
 * @brief Destroys the brodnik_vector.
 *
 * Deallocates all the memory used by the data blocks and the index block.
 */

template <class T> brodnik_vector<T>::~brodnik_vector() {
  for (int i = 0; i < this->ib_size; i++) {
    if (this->index_block[i] == nullptr)
      continue;
    delete[] this->index_block[i];
  }
  if (this->index_block != nullptr)
    delete[] this->index_block;
}

/**
 * @brief Grows the vector by adding a new element.
 *
 * Manages the allocation of data blocks and superblocks to accommodate the new element.
 * This involves increasing the size of the last data block, and if necessary,
 * allocating new data blocks and superblocks. The index block is also grown if needed
 * to hold pointers to new data blocks.
 */

template <class T> void brodnik_vector<T>::grow() {
  DEBUG_THIS("START-GROW");
  // Check if the current data block is full
  if (this->db_size == this->db_max_size) {
    // Check if the current superblock is full
    if (this->sb_size == this->sb_max_size) {
      this->sb_index++; // Increment s
      if (this->sb_index % 2)
        this->db_max_size *= 2; // s even
      else
        this->sb_max_size *= 2; // s odd
      this->sb_size = 0;
    }

    // Check if a new data block is needed and if there is space in the index block
    if (this->db_index + 2 > this->ib_size) {
      if (this->ib_size == this->ib_max_size) {
        T **new_index_block = new T *[2 * this->ib_size];
        for (int i = 0; i < this->ib_size; i++)
          new_index_block[i] = this->index_block[i];

        delete[] this->index_block;
        this->index_block = nullptr;
        this->index_block = new_index_block;

        this->ib_max_size *= 2;
      }
      // Allocate a new data block and add its pointer to the index block
      this->index_block[this->ib_size] = new T[this->db_max_size];
      this->ib_size++;
    }

    // Move to the next data block and reset the current data block size
    this->db_index++; 
    this->db_size = 0;

    this->sb_size++;
  }

  this->db_size++;
  this->n_size++;

  DEBUG_THIS("END-GROW");
}

/**
 * @brief Shrinks the vector by removing the last element.
 *
 * Decreases the element count and potentially deallocates the last data block
 * if it becomes empty. It also manages the index block and superblock sizes,
 * deallocating memory and adjusting indices as needed.
 */
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
    if (this->ib_size * 4 <= this->ib_max_size) {
      this->ib_max_size = this->ib_size;
      T **new_index_block = new T *[this->ib_size];
      for (int i = 0; i < this->ib_size; i++)
        new_index_block[i] = this->index_block[i];

      delete[] this->index_block;
      this->index_block = new_index_block;
    }

    // Decrement index of last used data block
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

/**
 * @brief Locates the data block and offset for a given index.
 *
 * This function implements the core logic of Brodnik's structure to
 * determine the correct data block and offset within that block for the element
 * at the given index `i`.
 *
 * @param i The index of the element to locate.
 * @return A reference to the element at the specified index.
 */
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

/**
 * @brief Adds an element to the end of the vector.
 *
 * Grows the vector and places the new value in the newly allocated space.
 *
 * @param value The value to be added to the vector.
 */
template <class T> void brodnik_vector<T>::push_back(T value) {
  this->grow();
  
  T &current_cell = this->locate(this->n_size - 1);
  
  current_cell = value;
}

/**
 * @brief Removes the last element from the vector.
 *
 * Shrinks the vector, deallocating memory if the last data block becomes empty.
 */
template <class T> void brodnik_vector<T>::pop_back() { this->shrink(); }

template<class T> bool brodnik_vector<T>::empty() const {
    return n_size == 0;
}

template<class T> T& brodnik_vector<T>::back() {
    return this->locate(n_size - 1);
}

template<class T> T& brodnik_vector<T>::front() {
  return this->locate(n_size - 1);
}

template<class T> T& brodnik_vector<T>::back() const {
  return this->locate(n_size - 1);
}

template<class T> T& brodnik_vector<T>::front() const {
return this->locate(n_size - 1);
}

template <class T> T& brodnik_vector<T>::operator[](int index) {
  return this->locate(index);
}

#endif
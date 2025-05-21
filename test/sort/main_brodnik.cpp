#include <algorithm>
#include <iostream>
#include "../../src/brodnik_vector_cpp/brodnik_vector.hpp"
#include "../debug.h"

using namespace std;

int main() {
  // Declare an integer to store the number of elements
  int numberOfElements;

  // Read the number of elements from standard input
  cin >> numberOfElements;

  // Declare a brodnik_vector of integers with the specified number of elements
  brodnik_vector<int> integerVector(numberOfElements);

  // Read the elements into the vector
  for (int i = 0; i < numberOfElements; ++i) {
    cin >> integerVector[i];
  }

  // Sort the elements in the vector
  sort(integerVector.begin(), integerVector.end());

  // Print the sorted elements
  for (int i = 0; i < numberOfElements; ++i) {
    PRINT_INT(integerVector[i]);
  }
  PRINT_END();

  PRINT_CHECK();
  
  return 0;
}

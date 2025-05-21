#include <algorithm>
#include <iostream>
#include <vector>
#include "../debug.h"

using namespace std;

int main() {
  // Declare an integer to store the number of elements
  int numberOfElements;

  // Read the number of elements from standard input
  cin >> numberOfElements;

  // Declare a vector of integers with the specified number of elements
  vector<int> integerVector(numberOfElements);

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
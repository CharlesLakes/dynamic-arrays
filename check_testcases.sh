#!/bin/bash

# Run Brodnik (C++) Sort Test
echo "Running Brodnik (C++) Sort Test..."
python3 check.py test/sort/main_brodnik.cpp test/sort/

echo "Running Brodnik (C) Sort Test..."
python3 check.py test/sort/main_brodnik.c test/sort/ gcc

echo "Running STL Vector (C++) Sort Test..."
python3 check.py test/sort/main_vector.cpp test/sort/

echo "Running Brodnik (C++) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.cpp test/binary_search/

echo "Running Brodnik (C) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.c test/binary_search/ gcc

echo "Running STL Vector (C++) Binary Search Test..."
python3 check.py test/binary_search/main_vector.cpp test/binary_search/

echo "Running Brodnik (C++) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.cpp test/linear_search/

echo "Running Brodnik (C) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.c test/linear_search/ gcc

echo "Running STL Vector (C++) Linear Search Test..."
python3 check.py test/linear_search/main_vector.cpp test/linear_search/

echo "Running Brodnik (C++) Stack Test..."
python3 check.py test/stack/main_brodnik.cpp test/stack/

echo "Running Brodnik (C) Stack Test..."
python3 check.py test/stack/main_brodnik.c test/stack/ gcc

echo "Running STL Stack (C++) Stack Test..."
python3 check.py test/stack/main_stack.cpp test/stack/

echo "All tests completed."

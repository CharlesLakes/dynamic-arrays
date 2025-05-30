#!/bin/bash

ulimit -s unlimited
ulimit -v unlimited
ulimit -t unlimited

# --- Begin:Stack ---

echo "Running Brodnik (C++) Stack Test..."
python3 check.py test/stack/main_brodnik.cpp test/stack/

echo "Running Brodnik (C) Stack Test..."
python3 check.py test/stack/main_brodnik.c test/stack/ gcc

echo "Running STL Stack Deque (C++) Stack Test..."
python3 check.py test/stack/main_stack_deque.cpp test/stack/

echo "Running STL Stack Vector (C++) Stack Test..."
python3 check.py test/stack/main_stack_vector.cpp test/stack/

echo "Running STL Stack List (C++) Stack Test..."
python3 check.py test/stack/main_stack_list.cpp test/stack/


# --- End:Stack ---

# --- Begin:Sort ---

echo "Running Brodnik (C++) Sort Test..."
python3 check.py test/sort/main_brodnik.cpp test/sort/

echo "Running Brodnik (C) Sort Test..."
python3 check.py test/sort/main_brodnik.c test/sort/ gcc

echo "Running STL Vector (C++) Sort Test..."
python3 check.py test/sort/main_vector.cpp test/sort/

echo "Running STL Deque (C++) Sort Test..."
python3 check.py test/sort/main_deque.cpp test/sort/

# --- End:Sort ---

# --- Begin:Binary Search ---

echo "Running Brodnik (C++) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.cpp test/binary_search/

echo "Running Brodnik (C) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.c test/binary_search/ gcc

echo "Running STL Vector (C++) Binary Search Test..."
python3 check.py test/binary_search/main_vector.cpp test/binary_search/

# --- End:Binary Search

# --- Begin:Linear Search ---

echo "Running Brodnik (C++) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.cpp test/linear_search/

echo "Running Brodnik (C) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.c test/linear_search/ gcc

echo "Running STL Vector (C++) Linear Search Test..."
python3 check.py test/linear_search/main_vector.cpp test/linear_search/

# --- End:Linear Search

# --- Begin:Heap ---

echo "Running Brodnik (C++) Heap Test..."
python3 check.py test/heap/main_brodnik.cpp test/heap/

echo "Running STL Vector (C++) Heap Test..."
python3 check.py test/heap/main_vector.cpp test/heap/

echo "Running STL Deque (C++) Heap Test..."
python3 check.py test/heap/main_deque.cpp test/heap/

# --- End:Heap

# --- Begin:Dijkstra ---

echo "Running Brodnik (C++) Dijkstra Test..."
#python3 check.py test/dijkstra/main_brodnik.cpp test/dijkstra/

echo "Running STL Vector (C++) Dijkstra Test..."
python3 check.py test/dijkstra/main_vector.cpp test/dijkstra/

echo "Running STL Deque (C++) Dijkstra Test..."
python3 check.py test/dijkstra/main_deque.cpp test/dijkstra/


# --- End:Dijkstra

echo "All tests completed."

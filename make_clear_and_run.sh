#!/bin/bash

ulimit -s unlimited
ulimit -v unlimited
ulimit -t unlimited

# --- Begin:Stack ---

python3 test/stack/generator-stats.py

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

rm -f test/**/*.in test/**/*.out

# --- End:Stack ---

# --- Begin:Sort ---

python3 test/sort/generator-stats.py

echo "Running Brodnik (C++) Sort Test..."
python3 check.py test/sort/main_brodnik.cpp test/sort/

echo "Running Brodnik (C) Sort Test..."
python3 check.py test/sort/main_brodnik.c test/sort/ gcc

echo "Running STL Vector (C++) Sort Test..."
python3 check.py test/sort/main_vector.cpp test/sort/

echo "Running STL Deque (C++) Sort Test..."
python3 check.py test/sort/main_deque.cpp test/sort/

rm -f test/**/*.in test/**/*.out

# --- End:Sort ---

# --- Begin:Binary Search ---

python3 test/binary_search/generator-stats.py

echo "Running Brodnik (C++) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.cpp test/binary_search/

echo "Running Brodnik (C) Binary Search Test..."
python3 check.py test/binary_search/main_brodnik.c test/binary_search/ gcc

echo "Running STL Vector (C++) Binary Search Test..."
python3 check.py test/binary_search/main_vector.cpp test/binary_search/

rm -f test/**/*.in test/**/*.out

# --- End:Binary Search

# --- Begin:Linear Search ---

python3 test/linear_search/generator-stats.py

echo "Running Brodnik (C++) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.cpp test/linear_search/

echo "Running Brodnik (C) Linear Search Test..."
python3 check.py test/linear_search/main_brodnik.c test/linear_search/ gcc

echo "Running STL Vector (C++) Linear Search Test..."
python3 check.py test/linear_search/main_vector.cpp test/linear_search/

rm -f test/**/*.in test/**/*.out

# --- End:Linear Search

# --- Begin:Heap ---

python3 test/heap/generator-stats.py

echo "Running Brodnik (C++) Heap Test..."
python3 check.py test/heap/main_brodnik.cpp test/heap/

echo "Running STL Vector (C++) Heap Test..."
python3 check.py test/heap/main_vector.cpp test/heap/

echo "Running STL Deque (C++) Heap Test..."
python3 check.py test/heap/main_deque.cpp test/heap/

rm -f test/**/*.in test/**/*.out

# --- End:Heap

# --- Begin:Dijkstra ---

python3 test/dijkstra/generator-stats.py

echo "Running Brodnik (C++) Heap Test..."
python3 check.py test/dijkstra/main_brodnik.cpp test/dijkstra/

echo "Running STL Vector (C++) Heap Test..."
python3 check.py test/dijkstra/main_vector.cpp test/dijkstra/

echo "Running STL Deque (C++) Heap Test..."
python3 check.py test/dijkstra/main_deque.cpp test/dijkstra/

rm -f test/**/*.in test/**/*.out

# --- End:Dijkstra

echo "All tests completed."

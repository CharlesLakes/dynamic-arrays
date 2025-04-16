#!/bin/bash

ulimit -s unlimited
ulimit -v unlimited
ulimit -t unlimited

# Sort
echo "Generating test data for Sort..."
python3 test/sort/generator-stats.py

echo "Generating test data for Stack..."
python3 test/stack/generator-stats.py

echo "Generating test data for Linear Search..."
python3 test/linear_search/generator-stats.py

echo "Generating test data for Binary Search..."
python3 test/binary_search/generator-stats.py
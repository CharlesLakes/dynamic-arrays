#!/bin/bash

ulimit -s unlimited
ulimit -v unlimited
ulimit -t unlimited

# Sort
echo "Generating test data for Sort..."
python3 test/sort/generator.py

echo "Generating test data for Stack..."
python3 test/stack/generator.py

echo "Generating test data for Linear Search..."
python3 test/linear_search/generator.py

echo "Generating test data for Binary Search..."
python3 test/binary_search/generator.py
# Brodnik's Data Structure: An STL-Compatible Implementation and Evaluation

This research project implements and evaluates Brodnik's data structure as a modern C++ STL-compatible container. The primary goal is to compare its performance against `std::vector` and `std::deque` across different algorithmic scenarios.

## Repository Structure

The repository is organized into the following main directories and files:

- **`src/`**: Contains the source code for the C and C++ implementations of Brodnik's data structure.
  - **`brodnik_vector_c/`**: C implementation.
  - **`brodnik_vector_cpp/`**: C++ STL-compatible implementation.
- **`test/`**: Includes test cases, test case generators, and Makefiles for compiling and running the benchmarks. Each subdirectory corresponds to a specific algorithmic test.
- **`stats/`**: Contains scripts and results related to performance analysis.
- **`Makefile`**: Main Makefile for managing compilations and tests.
- **`check.py`**: Python script for checking test results.
- **`requirements.txt`**: Python dependencies for the project.

## Test Case Instructions

For detailed instructions on how to generate test cases for each of the supported algorithmic scenarios, please refer to the `README.md` file in the respective test directory:

- **Sort**: See instructions in [`test/sort/README.md`](./test/sort/README.md).
- **Stack**: See instructions in [`test/stack/README.md`](./test/stack/README.md).
- **Dijkstra**: See instructions in [`test/dijkstra/README.md`](./test/dijkstra/README.md).
- **Heap**: See instructions in [`test/heap/README.md`](./test/heap/README.md).

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

This section explains how to generate the test cases for each of the supported algorithmic scenarios. For more detailed information, refer to the `README.md` file located in each respective test directory.

### Sort

The sort tests evaluate container performance with `std::sort` operations.

**Test Case Generation:**
1.  Navigate to the `test/sort/` directory.
2.  Compile the test case generator:
    ```sh
    g++ -Ofast testcase-generator.cpp -o main
    ```
3.  Run the generator:
    - For a random seed: `./main`
    - For a specific seed: `./main --seed 12345`

### Stack

The stack tests evaluate container performance in LIFO (Last-In, First-Out) scenarios.

**Test Case Generation:**
1.  Navigate to the `test/stack/` directory.
2.  The generator has three modes. You can compile for a specific mode or all modes at once.
    - **Compile a specific mode (e.g., Mode 1):**
      ```sh
      g++ -DMODE_1 -Ofast testcase-generator.cpp -o main
      ```
    - **Compile all modes:**
      ```sh
      g++ -DMODE_1 -DMODE_2 -DMODE_3 -Ofast testcase-generator.cpp -o main
      ```
    - **Using Make (recommended):**
      - `make mode_1`, `make mode_2`, or `make mode_3` for specific modes.
      - `make mode_all` for all modes.
3.  Run the generator:
    - For a random seed: `./main`
    - For a specific seed: `./main --seed 12345`

### Dijkstra

The Dijkstra tests evaluate container performance as the underlying storage for a `std::priority_queue` in Dijkstra's shortest path algorithm.

**Test Case Generation:**
1.  Navigate to the `test/dijkstra/` directory.
2.  Compile the test case generator:
    ```sh
    g++ -Ofast testcase-generator.cpp -o main
    ```
3.  Run the generator:
    - For a random seed: `./main`
    - For a specific seed: `./main --seed 12345`

### Heap

The heap tests evaluate container performance with `std::priority_queue` operations.

**Test Case Generation:**
1.  Navigate to the `test/heap/` directory.
2.  Compile the test case generator:
    ```sh
    g++ -Ofast testcase-generator.cpp -o main
    ```
3.  Run the generator:
    - For a random seed: `./main`
    - For a specific seed: `./main --seed 12345`

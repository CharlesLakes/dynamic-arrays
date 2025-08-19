# Dynamic Arrays: Implementation and Performance Evaluation

This research project provides a framework for implementing and evaluating dynamic array data structures, with a focus on Brodnik's theoretically optimal structure as an STL-compatible container. While primarily emphasizing Brodnik's implementation, this repository serves as a platform for comparing various dynamic array structures against standard C++ containers like `std::vector` and `std::deque`.

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

- **Sort**: See instructions in [`test/sort/`](./test/sort/).
- **Stack**: See instructions in [`test/stack/`](./test/stack/).
- **Dijkstra**: See instructions in [`test/dijkstra/`](./test/dijkstra/).
- **Heap**: See instructions in [`test/heap/`](./test/heap/).

# Run Code with Test Cases

```sh
make run
```

The above command automatically runs the test cases generated in the `test/*/*` directories.
The execution mechanism tests all files with the `.in` extension.

## Contributing

We welcome contributions to expand the framework with additional dynamic array implementations or enhance the existing benchmarking suite.

### Contribution Guidelines

1. **Fork the repository** and create a feature branch
2. **Follow the existing code style** and project structure
3. **Add comprehensive tests** for new implementations
4. **Update documentation** as needed
5. **Submit a pull request** with a descriptive title following GitHub conventions

### Pull Request Standards

- **Squash and merge** strategy is applied to maintain clean commit history
- **Title format**: Use conventional commit format (e.g., `feat: add new dynamic array implementation`, `fix: resolve memory leak in brodnik`, `docs: update benchmark instructions`)

### Areas for Contribution

- **New Data Structures**: Implement additional dynamic array variants
- **Optimization**: Hardware-specific optimizations and performance improvements
- **Benchmarking**: Extended test scenarios and real-world application benchmarks
- **Analysis Tools**: Enhanced performance visualization and statistical analysis
- **Documentation**: Improved guides and theoretical explanations
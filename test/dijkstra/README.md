# Dijkstra

## Test Case Generator

To generate test cases for Dijkstra's shortest path algorithm, compile the `testcase-generator.cpp` file. The generator creates graph datasets for evaluating container performance with `std::priority_queue` operations in Dijkstra's algorithm.

### Compilation

```sh
g++ -Ofast testcase-generator.cpp -o main
```

### Execution

- **Automatic seed**: Run without arguments to generate a random seed
- **Custom seed**: Specify a seed for reproducible results:

```sh
./main --seed 12345
```

### Test Case Specifications

The generator creates two types of graph benchmarks:

#### Sparse Graphs
- **Nodes**: 10^n (n = 1 to 8)
- **Edges**: O(N) density (2N edges)
- **Structure**: Connected with linear chain backbone

#### Dense Graphs
- **Nodes**: 10^n (n = 1 to 4)
- **Edges**: O(N²) density (up to N²/4 edges)
- **Structure**: Connected with linear chain backbone

### Generated Files

Each `.in` file contains:
- First line: Number of nodes (N) and edges (E)
- Following lines: Edge specifications `u v weight`

File naming: `T{n}_sparse.in` and `T{n}_dense.in`

These test cases evaluate how different containers (brodnik, std::vector, std::deque) perform as underlying storage for `std::priority_queue` in Dijkstra's algorithm.
# Heap

## Test Case Generator

To generate test cases for priority queue operations, compile the `testcase-generator.cpp` file. The generator creates test datasets for evaluating container performance with `std::priority_queue` operations using different underlying containers.

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

The generator creates priority queue benchmarks with initial heap population followed by mixed push/pop operations.

Each generated `.in` file contains:
- Number of initial elements and operations
- Initial integers and operation sequences (`1 value` for push, `0` for pop)

These test cases evaluate heap performance when different containers serve as underlying storage for priority queues.
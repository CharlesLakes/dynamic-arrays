# Sort

## Test Case Generator

To generate test cases for sorting algorithms, compile the `testcase-generator.cpp` file. The generator creates large-scale test datasets for evaluating container performance with `std::sort` operations.

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

The generator creates sorting benchmarks with randomly generated arrays using uniformly distributed integer values. Array sizes can be configured by modifying the loop parameters in `main()`. 

Each generated `.in` file contains:
- First line: Number of elements (N)
- Following lines: N integers for sorting

These test cases evaluate random access performance and cache characteristics when different containers serve as underlying storage for sorting algorithms.
# Stack

## Test Case Generators

To generate test cases, you need to compile the `testcase-generator.cpp` file. Since the test cases for stack are very large, 3 different modes have been defined, where each mode groups different types of test cases:

### Available Modes

- **Mode 1 - Random Operations and Push-Random**: 
  - `random_ops_testcase`: Completely random sequence of push/pop operations
  - `push_then_random_ops_testcase`: Initial insertion followed by random operations

- **Mode 2 - Random-Pop and Push-Random-Pop Patterns**: 
  - `random_then_pop_ops_testcase`: Random operations followed by sequential extractions
  - `generate_push_random_pop_ops_testcase`: Complete pattern of insertion, random operations, and extraction

- **Mode 3 - Cyclic Operations**:
  - `alternating_push_pop_ops_testcase`: Alternating cycles of massive insertion and extraction

### Compilation

#### Mode-specific compilation
To compile a specific mode, use the following command:

```sh
g++ -DMODE_i -Ofast testcase-generator.cpp -o main
```

Where `i` is the desired mode number.

**Example for mode 1:**
```sh
g++ -DMODE_1 -Ofast testcase-generator.cpp -o main
```

#### All modes compilation
To compile all modes simultaneously:

```sh
g++ -DMODE_1 -DMODE_2 -DMODE_3 -Ofast testcase-generator.cpp -o main
```

#### Using Make commands (recommended)
For greater convenience, you can use the predefined Make commands:

**For a specific mode:**
```sh
make mode_i
```

**For all modes:**
```sh
make mode_all
```

### Execution

- **Automatic seed**: If you run the program without arguments, a random seed is automatically generated.
- **Custom seed**: To use a specific seed, add the `--seed` argument followed by the desired value:

```sh
./main --seed 12345
```

## Seeds

* "Brodnik’s Data Structure in Practice, Revisited":
  * Mode 1: 2613726499
  * Mode 2: 1969640844
  * Mode 3: 2839142279
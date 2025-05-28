# Dynamic Vector and Algorithm Benchmarking

This repository contains code for benchmarking various dynamic vector implementations and common algorithms. It provides tools to test their performance and analyze the results.

## Repository Structure

The repository is organized as follows:

*   **`.github/workflows/`**: Contains GitHub Actions workflows for automated testing of different algorithms (binary search, Dijkstra, heap, linear search, sort, stack).
*   **`src/`**: Contains the source code for data structure implementations.
    *   `brodnik_vector_c/`: Implementation of Brodnik's dynamic vector in C.
    *   `brodnik_vector_cpp/`: Implementation of Brodnik's dynamic vector in C++.
*   **`test/`**: Contains test code, test case generators, and main files for benchmarking different algorithms. Each subdirectory corresponds to a specific algorithm or data structure being tested.
    *   `binary_search/`
    *   `dijkstra/`
    *   `heap/`
    *   `linear_search/`
    *   `sort/`
    *   `stack/`
*   **`stats/`**: Contains scripts and files related to statistics generation and analysis.
    *   `main.py`: Python script to parse test output, extract metrics, and generate plots (using `matplotlib`).
    *   `main.md`: Markdown file likely used for summarizing or displaying results.
    *   `stages/`: Text files that seem to store raw output from different stages of testing.
*   **`Makefile`**: Provides `make` targets for building the code and running the benchmarks.
*   **`check.py`**: A Python script, likely for checking results or code sanity.
*   **`check_testcases.sh`**: Shell script for checking test cases.
*   **`make_clear_and_run.sh`**: Shell script to clean and then run tests.
*   **`make_testcases.sh`**: Shell script to generate test cases.
*   **`requirements.txt`**: Lists the Python dependencies required for the project (e.g., for `stats/main.py`).

## Getting Started

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```
2.  **Install dependencies (if running statistics generation):**
    ```bash
    pip install -r requirements.txt
    ```
3.  **Build and run tests:**
    The `Makefile` likely provides targets such as `make all` or specific targets for each test. Explore the `Makefile` for available commands.
    Example:
    ```bash
    make # This might build and run a default set of tests
    ```
    You can also use the provided shell scripts like `./make_clear_and_run.sh`.

4.  **Generating Statistics:**
    The `stats/main.py` script can be used to process the output of the benchmarks. It typically reads data from files in the `stats/stages/` directory.
    ```bash
    python stats/main.py <path_to_stage_output_file>
    ```
    This will generate plots (e.g., `_cpu_user.png`, `_memory.png`) in the root directory.

## How to Interpret Results

The benchmarks typically measure:
*   **Wall Time:** Total time elapsed.
*   **CPU User Time:** Time spent in user-mode code.
*   **CPU System Time:** Time spent in kernel-mode code.
*   **Max Memory Usage:** Peak memory consumption during the test.

The `stats/main.py` script visualizes these metrics, allowing for comparison between different implementations or algorithms under various test conditions (input sizes `n`, test case types).

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change. Please make sure to update tests as appropriate.
Commit messages for pull requests should follow the existing conventions in the repository.

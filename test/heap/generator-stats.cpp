#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include <cmath>     // For std::pow
#include <algorithm> // For std::min (though not strictly needed here, good include for generators)
#include <iomanip>   // For std::put_time (used by generator_utils.hpp)
#include <chrono>    // For std::chrono (used by generator_utils.hpp)

#include "../generator_utils.hpp" // Adjust path if necessary

// Global variables for file writing (kept for now to maintain exact behavior)
std::string CODE_DIR;
std::set<std::string> file_cleaned;
int count_write = 0;
std::string write_acumulation = "";

// Function to create or clear a file
void create_or_clear(const std::string& filename) {
    std::ofstream file(CODE_DIR + "/" + filename + ".in");
    file << ""; // Clear the file
    file.close();
}

// Function to write content to a file with custom buffering
void write(const std::string& filename, const std::string& content) {
    count_write++;
    count_write %= 1000000; // Buffer up to 1,000,000 writes or characters

    write_acumulation += content;

    if (count_write != 0) { // Only write to file when buffer limit is reached or explicitly flushed
        return;
    }

    if (file_cleaned.find(filename) == file_cleaned.end()) {
        create_or_clear(filename);
        file_cleaned.insert(filename);
    }

    std::ofstream file(CODE_DIR + "/" + filename + ".in", std::ios::app);
    file << write_acumulation;
    file.close();

    write_acumulation = ""; // Reset accumulation buffer
}

// Function to flush any remaining content in write_acumulation
void flush_write_buffer(const std::string& filename) {
    if (!write_acumulation.empty()) {
        if (file_cleaned.find(filename) == file_cleaned.end()) {
            create_or_clear(filename);
            file_cleaned.insert(filename);
        }
        std::ofstream file(CODE_DIR + "/" + filename + ".in", std::ios::app);
        file << write_acumulation;
        file.close();
        write_acumulation = "";
    }
    count_write = 0; // Reset counter after flush
}

/**
 * @brief Generates a single test case for the heap.
 *
 * @param n_factor The scale factor for the size of the test case (10^n_factor elements/operations).
 * @param rand_gen Reference to the RandomGenerator instance.
 */
void generate_heap_testcase(int n_factor, RandomGenerator& rand_gen) {
    int size = static_cast<int>(std::pow(10, n_factor));
    std::string case_name = "T" + std::to_string(n_factor);

    // Part 1: Initial numbers to populate the heap or test with
    write(case_name, std::to_string(size) + "\n");
    for (int i = 0; i < size; ++i) {
        write(case_name,
              std::to_string(rand_gen.nextInt(1, 1000000000)) +
              (i < size - 1 ? " " : "\n")); // Add space or newline
    }

    // Part 2: Operations to perform
    // Number of operations is typically related to the size, e.g., size-1 or size
    int num_operations = (size > 0) ? (size -1) : 0; // Avoid negative operations for size 0
    if (size == 0 && n_factor == 0) num_operations = 1; // Special case for T0 if needed, or adjust
    if (size == 1 && n_factor ==1) num_operations = 1; // T1, size 10, 9 ops. if size is 1, 0 ops. Let's make it 1 op for size 1.

    write(case_name, std::to_string(num_operations) + "\n");
    for (int i = 0; i < num_operations; ++i) {
        int op_type = rand_gen.nextInt(0, 1); // 0 for pop/extract, 1 for push/insert
        write(case_name, std::to_string(op_type));
        if (op_type == 1) { // If it's a push operation, generate a value
            write(case_name, " " + std::to_string(rand_gen.nextInt(1, 1000000000)));
        }
        write(case_name, "\n");
    }

    flush_write_buffer(case_name); // Ensure all buffered content for this test case is written
}

int main(int argc, char* argv[]) {
    // Initialize random number generator using command line arguments
    RandomGenerator randomGenerator(argc, argv);

    // Determine the base directory for output files
    try {
        CODE_DIR = std::filesystem::current_path().string();
        if (CODE_DIR.empty()) {
            CODE_DIR = "."; // Fallback to current directory
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Warning: Could not get current path: " << e.what() << ". Using '.' as CODE_DIR." << std::endl;
        CODE_DIR = ".";
    }

    // Write seed information to a file in the CODE_DIR
    randomGenerator.writeSeedToFile(CODE_DIR);

    // Generate test cases of different sizes
    // n_factor from 1 to 8, meaning sizes from 10^1 to 10^8
    // Smallest test case (n=0 for 10^0 = 1 element) can be useful too.
    // Let's adjust to n=0 to 8 for 1 to 10^8 elements.
    for (int n = 1; n < 9; ++n) { // n_factor from 0 to 8
        std::cout << "Generating heap test case for N_factor = " << n << "..." << std::endl;
        generate_heap_testcase(n, randomGenerator);
    }

    std::cout << "Heap test case generation complete." << std::endl;
    return 0;
}
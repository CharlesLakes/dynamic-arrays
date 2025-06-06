#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include <cmath>     // For std::pow
#include <algorithm> // For std::min, std::max etc. (though not strictly needed here, good include for generators)
#include <iomanip>   // For std::put_time (used by generator_utils.hpp)
#include <chrono>    // For std::chrono (used by generator_utils.hpp)

#include "../generator_utils.hpp"

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
 * @brief Generates a single test case for sorting.
 *
 * @param n_factor The scale factor for the size of the test case (10^n_factor elements).
 * @param rand_gen Reference to the RandomGenerator instance.
 */
void generate_sort_testcase(int n_factor, RandomGenerator& rand_gen) {
    // Determine the number of elements based on n_factor.
    // Original loop was for n = 1 to 9 (exclusive), meaning 10^1 to 10^8.
    // If n_factor is 0, size is 1. If n_factor is 1, size is 10.
    int size = static_cast<int>(std::pow(10, n_factor));
    if (n_factor == 0) size = 1; // Explicitly for 10^0 = 1 element case

    std::string case_name = "T" + std::to_string(n_factor);

    write(case_name, std::to_string(size) + "\n");

    for (int i = 0; i < size; ++i) {
        write(case_name,
              std::to_string(rand_gen.nextInt(1, 1000000000)) +
              (i < size - 1 ? " " : "\n")); // Add space between numbers, newline at the end
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

    // Generate test cases of different sizes.
    // Original loop: n = 1 to 9 (exclusive), i.e., 10^1 to 10^8.
    // We'll use n_factor from 1 to 8 to match this.
    // Or, to include 10^0, n_factor from 0 to 8. Let's use 0 to 8.
    for (int n = 1; n < 9; ++n) { // n_factor from 0 to 8 (generates T0 to T8)
                                 // T0: 10^0=1 element, T1: 10^1=10 elements, ..., T8: 10^8 elements
        std::cout << "Generating sort test case for N_factor = " << n << "..." << std::endl;
        generate_sort_testcase(n, randomGenerator);
    }

    std::cout << "Sort test case generation complete." << std::endl;
    return 0;
}
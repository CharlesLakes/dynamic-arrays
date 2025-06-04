#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include <cmath>     // For std::pow
#include <algorithm> // For std::min, std::max
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
    // file.write("", 0); // This is one way, but simple clearing is fine
    file << "";
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

// --- Test Case Generation Functions ---

/**
 * @brief Writes the initial array of numbers for a test case.
 * @param case_name_suffix Suffix for the test case file name (e.g., "_random").
 * @param n_factor Scale factor for size.
 * @param rand_gen Reference to the RandomGenerator.
 * @return The calculated size (10^n_factor).
 */
int generate_initial_numbers(const std::string& case_name_suffix, int n_factor, RandomGenerator& rand_gen) {
    int size = static_cast<int>(std::pow(10, n_factor));
    if (n_factor == 0) size = 1;
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;

    write(full_case_name, std::to_string(size) + "\n");
    for (int i = 0; i < size; ++i) {
        write(full_case_name,
              std::to_string(rand_gen.nextInt(1, 1000000000)) +
              (i < size - 1 ? " " : "\n"));
    }
    return size;
}

void generate_random_ops_testcase(int n_factor, RandomGenerator& rand_gen) {
    std::string case_name_suffix = "_random";
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;
    int size = generate_initial_numbers(case_name_suffix, n_factor, rand_gen);

    write(full_case_name, std::to_string(size) + "\n"); // Number of operations
    for (int i = 0; i < size; ++i) {
        // Operation: 0 for pop (value is ignored), 1 for push
        int op_type = rand_gen.nextInt(0, 1);
        std::string op_str = std::to_string(op_type);
        if (op_type == 1) { // Push operation
            op_str += " " + std::to_string(rand_gen.nextInt(1, 1000000000));
        } else { // Pop operation, can add a dummy value like 0 or not, depending on parser
            op_str += " 0"; // Assuming parser expects a value even for pop
        }
        write(full_case_name, op_str + "\n");
    }
    flush_write_buffer(full_case_name);
}

void generate_push_then_random_ops_testcase(int n_factor, RandomGenerator& rand_gen) {
    std::string case_name_suffix = "_push_then_random";
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;
    int size = generate_initial_numbers(case_name_suffix, n_factor, rand_gen);

    int num_queries = (size / 2) * 2; // Ensure it's even for splitting
    if (size == 1) num_queries = 1; // Handle small N
    if (size == 0) num_queries = 0;

    write(full_case_name, std::to_string(num_queries) + "\n");
    int push_count = num_queries / 2;
    int random_count = num_queries - push_count;

    for (int i = 0; i < push_count; ++i) { // Push operations
        write(full_case_name, "1 " + std::to_string(rand_gen.nextInt(1, 1000000000)) + "\n");
    }
    for (int i = 0; i < random_count; ++i) { // Random operations
        int op_type = rand_gen.nextInt(0, 1);
        std::string op_str = std::to_string(op_type);
        if (op_type == 1) {
            op_str += " " + std::to_string(rand_gen.nextInt(1, 1000000000));
        } else {
            op_str += " 0";
        }
        write(full_case_name, op_str + "\n");
    }
    flush_write_buffer(full_case_name);
}

void generate_random_then_pop_ops_testcase(int n_factor, RandomGenerator& rand_gen) {
    std::string case_name_suffix = "_random_then_pop";
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;
    int size = generate_initial_numbers(case_name_suffix, n_factor, rand_gen);
    
    int num_queries = (size / 2) * 2;
    if (size == 1) num_queries = 1;
    if (size == 0) num_queries = 0;

    write(full_case_name, std::to_string(num_queries) + "\n");
    int random_count = num_queries / 2;
    int pop_count = num_queries - random_count;

    for (int i = 0; i < random_count; ++i) { // Random operations
        int op_type = rand_gen.nextInt(0, 1);
        std::string op_str = std::to_string(op_type);
        if (op_type == 1) {
            op_str += " " + std::to_string(rand_gen.nextInt(1, 1000000000));
        } else {
            op_str += " 0";
        }
        write(full_case_name, op_str + "\n");
    }
    for (int i = 0; i < pop_count; ++i) { // Pop operations
        write(full_case_name, "0 0\n");
    }
    flush_write_buffer(full_case_name);
}

void generate_push_random_pop_ops_testcase(int n_factor, RandomGenerator& rand_gen) {
    std::string case_name_suffix = "_push_random_then_pop";
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;
    int size = generate_initial_numbers(case_name_suffix, n_factor, rand_gen);

    int num_queries = (size / 3) * 3; // Ensure divisible by 3
    if (size < 3 && size > 0) num_queries = size;
    if (size == 0) num_queries = 0;

    write(full_case_name, std::to_string(num_queries) + "\n");
    int third = num_queries / 3;

    for (int i = 0; i < third; ++i) { // Push operations
        write(full_case_name, "1 " + std::to_string(rand_gen.nextInt(1, 1000000000)) + "\n");
    }
    for (int i = 0; i < third; ++i) { // Random operations
        int op_type = rand_gen.nextInt(0, 1);
        std::string op_str = std::to_string(op_type);
        if (op_type == 1) {
            op_str += " " + std::to_string(rand_gen.nextInt(1, 1000000000));
        } else {
            op_str += " 0";
        }
        write(full_case_name, op_str + "\n");
    }
    for (int i = 0; i < (num_queries - 2*third) ; ++i) { // Pop operations (remaining part)
        write(full_case_name, "0 0\n");
    }
    flush_write_buffer(full_case_name);
}

void generate_alternating_push_pop_ops_testcase(int n_factor, RandomGenerator& rand_gen) {
    std::string case_name_suffix = "_push_then_pop"; // Original name suggests alternating blocks
    std::string full_case_name = "T" + std::to_string(n_factor) + case_name_suffix;
    int size = generate_initial_numbers(case_name_suffix, n_factor, rand_gen);

    std::vector<int> blocks;
    int total_queries = 0;
    int current_stack_size = size; // Theoretical size after initial numbers (not used by stack ops directly here)

    for (int j = 0; j < 10; ++j) { // 10 blocks of operations
        int block_size_rand_limit = std::max(size / 10, 1); // Ensure limit is at least 1
        if (size == 0 && n_factor == 0) block_size_rand_limit =1; // if size is 0, make limit 1
        
        int random_block_size = rand_gen.nextInt(block_size_rand_limit, std::max(size,1)); // Ensure max is at least 1
        if (size == 0 && n_factor == 0) random_block_size = rand_gen.nextInt(1,2);


        if (j % 2 == 0) { // Push block
            current_stack_size += random_block_size;
        } else { // Pop block
            random_block_size = std::min(random_block_size, std::max(current_stack_size, 0));
            current_stack_size -= random_block_size;
        }
        blocks.push_back(random_block_size);
        total_queries += random_block_size;
    }
    if (size == 0 && n_factor == 0 && total_queries == 0 && blocks.empty()){ // ensure at least one op for T0
        blocks.push_back(1); // push 1
        total_queries = 1;
    }


    write(full_case_name, std::to_string(total_queries) + "\n");

    for (size_t j = 0; j < blocks.size(); ++j) {
        int block_size = blocks[j];
        bool is_push_block = (j % 2 == 0);
        for (int k = 0; k < block_size; ++k) {
            if (is_push_block) {
                write(full_case_name, "1 " + std::to_string(rand_gen.nextInt(1, 1000000000)) + "\n");
            } else {
                write(full_case_name, "0 0\n");
            }
        }
    }
    flush_write_buffer(full_case_name);
}


int main(int argc, char* argv[]) {
    // Initialize random number generator
    RandomGenerator randomGenerator(argc, argv);

    // Determine base directory
    try {
        CODE_DIR = std::filesystem::current_path().string();
        if (CODE_DIR.empty()) {
            CODE_DIR = "."; // Fallback
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Warning: Could not get current path: " << e.what() << ". Using '.' as CODE_DIR." << std::endl;
        CODE_DIR = ".";
    }

    randomGenerator.writeSeedToFile(CODE_DIR);

    // Generate test cases (n_factor from 0 to 8 for T0 to T8)
    // T0 will have size 1 (10^0), T1 size 10, ..., T8 size 10^8
    for (int n = 1; n < 10; ++n) {
        std::cout << "Generating stack test cases for N_factor = " << n << "..." << std::endl;
        // Mode 1
        generate_random_ops_testcase(n, randomGenerator);
        generate_push_then_random_ops_testcase(n, randomGenerator);
        // Mode 2
        generate_random_then_pop_ops_testcase(n, randomGenerator);
        generate_push_random_pop_ops_testcase(n, randomGenerator);
        // Mode 3
        generate_alternating_push_pop_ops_testcase(n, randomGenerator);
    }

    std::cout << "Stack test case generation complete." << std::endl;
    return 0;
}

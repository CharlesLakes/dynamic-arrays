
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <random>
#include <cmath>
#include <filesystem>
#include <sstream>
#include <vector>
#include <algorithm>

// Global variables
std::set<std::string> file_cleaned;
int count_write = 0;
std::string write_acumulation = "";
std::string CODE_DIR;

// Random number generation setup
std::random_device rd;
std::mt19937 gen(rd());

// Function to get random integer in range [min, max] (inclusive)
int randint(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void create_or_clear(const std::string& filename) {
    std::ofstream file(CODE_DIR + "/" + filename + ".in");
    file.write("", 0);
    file.close();
}

void write(const std::string& filename, const std::string& content) {
    count_write++;
    count_write %= 1000000;
    
    write_acumulation += content;
    
    if (count_write != 0) {
        return;
    }
    
    if (file_cleaned.find(filename) == file_cleaned.end()) {
        create_or_clear(filename);
        file_cleaned.insert(filename);
    }
    
    std::ofstream file(CODE_DIR + "/" + filename + ".in", std::ios::app);
    file << write_acumulation;
    file.close();
    
    write_acumulation = "";
}

int main() {
    // Get current directory (approximation of Python's approach)
    CODE_DIR = std::filesystem::current_path().string();
    
    // --- random ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));
        
        write(
            "T" + std::to_string(n) + "_random",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_random",
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        write(
            "T" + std::to_string(n) + "_random",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_random",
                std::to_string(randint(0, 1)) + " " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_random", "");
    }
    
    // --- push then random ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));
        
        write(
            "T" + std::to_string(n) + "_push_then_random",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_push_then_random",
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 2) * 2;
        
        write(
            "T" + std::to_string(n) + "_push_then_random",
            std::to_string(queries) + "\n"
        );
        
        // -- push --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + std::to_string(n) + "_push_then_random",
                "1 " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- random --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + std::to_string(n) + "_push_then_random",
                std::to_string(randint(0, 1)) + " " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_push_then_random", "");
    }
    
    // -- random then pop --
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));
        
        write(
            "T" + std::to_string(n) + "_random_then_pop",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_random_then_pop",
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 2) * 2;
        
        write(
            "T" + std::to_string(n) + "_random_then_pop",
            std::to_string(queries) + "\n"
        );
        
        // -- random --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + std::to_string(n) + "_random_then_pop",
                std::to_string(randint(0, 1)) + " " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- pop --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + std::to_string(n) + "_random_then_pop",
                "0 0\n"
            );
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_random_then_pop", "");
    }
    
    // --- push random then pop ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));
        
        write(
            "T" + std::to_string(n) + "_push_random_then_pop",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_push_random_then_pop",
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 3) * 3;
        
        write(
            "T" + std::to_string(n) + "_push_random_then_pop",
            std::to_string(queries) + "\n"
        );
        
        // -- push --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + std::to_string(n) + "_push_random_then_pop",
                "1 " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- random --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + std::to_string(n) + "_push_random_then_pop",
                std::to_string(randint(0, 1)) + " " + std::to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- pop --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + std::to_string(n) + "_push_random_then_pop",
                "0 0\n"
            );
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_push_random_then_pop", "");
    }
    
    // A_0 push + A_1 pop ...
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));
        
        std::vector<int> blocks_size;
        
        int temporal_size = size;
        int max_block_size = size / (2 * n);
        while (temporal_size > 0) {
            int left = std::min(static_cast<int>(std::pow(size, 2.0/3.0)), temporal_size);
            int right = std::min(max_block_size, temporal_size);
            
            if (left > right) {
                int pivot = left;
                left = right;
                right = pivot;
            }
            
            int block = randint(left, right);
            blocks_size.push_back(block);
            
            temporal_size -= block;
        }
        
        write(
            "T" + std::to_string(n) + "_push_then_pop",
            std::to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n) + "_push_then_pop",
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int sum_blocks = 0;
        for (int block : blocks_size) {
            sum_blocks += block;
        }
        
        write(
            "T" + std::to_string(n) + "_push_then_pop",
            std::to_string(sum_blocks) + "\n"
        );
        
        for (int i = 0; i < static_cast<int>(blocks_size.size()); i++) {
            int block = blocks_size[i];
            
            if (i % 2 == 0) {
                // -- push --
                for (int j = 0; j < block; j++) {
                    write(
                        "T" + std::to_string(n) + "_push_then_pop",
                        "1 " + std::to_string(randint(1, 1000000000)) + "\n"
                    );
                }
            } else {
                // -- pop --
                for (int j = 0; j < block; j++) {
                    write(
                        "T" + std::to_string(n) + "_push_then_pop",
                        "0 0\n"
                    );
                }
            }
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_push_then_pop", "");
    }
    
    return 0;
}


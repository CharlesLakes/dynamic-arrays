#include <bits/stdc++.h>

using namespace std;

// Global variables
set<string> file_cleaned;
int count_write = 0;
string write_acumulation = "";
string CODE_DIR;

// Random number generation setup
random_device rd;
mt19937 gen(rd());

// Function to get random integer in range [min, max] (inclusive)
int randint(int min, int max) {
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void create_or_clear(const string& filename) {
    ofstream file(CODE_DIR + "/" + filename + ".in");
    file.write("", 0);
    file.close();
}

void write(const string& filename, const string& content) {
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
    
    ofstream file(CODE_DIR + "/" + filename + ".in", ios::app);
    file << write_acumulation;
    file.close();
    
    write_acumulation = "";
}

int main() {
    // Get current directory (approximation of Python's approach)
    CODE_DIR = filesystem::current_path().string();
    
    // --- random ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(pow(10, n));
        
        write(
            "T" + to_string(n) + "_random",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_random",
                to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        write(
            "T" + to_string(n) + "_random",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_random",
                to_string(randint(0, 1)) + " " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        count_write = -1;
        write("T" + to_string(n) + "_random", "");
    }
    
    // --- push then random ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(pow(10, n));
        
        write(
            "T" + to_string(n) + "_push_then_random",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_push_then_random",
                to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 2) * 2;
        
        write(
            "T" + to_string(n) + "_push_then_random",
            to_string(queries) + "\n"
        );
        
        // -- push --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + to_string(n) + "_push_then_random",
                "1 " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- random --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + to_string(n) + "_push_then_random",
                to_string(randint(0, 1)) + " " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        count_write = -1;
        write("T" + to_string(n) + "_push_then_random", "");
    }
    
    // -- random then pop --
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(pow(10, n));
        
        write(
            "T" + to_string(n) + "_random_then_pop",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_random_then_pop",
                to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 2) * 2;
        
        write(
            "T" + to_string(n) + "_random_then_pop",
            to_string(queries) + "\n"
        );
        
        // -- random --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + to_string(n) + "_random_then_pop",
                to_string(randint(0, 1)) + " " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- pop --
        for (int i = 0; i < size / 2; i++) {
            write(
                "T" + to_string(n) + "_random_then_pop",
                "0 0\n"
            );
        }
        
        count_write = -1;
        write("T" + to_string(n) + "_random_then_pop", "");
    }
    
    // --- push random then pop ---
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(pow(10, n));
        
        write(
            "T" + to_string(n) + "_push_random_then_pop",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_push_random_then_pop",
                to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int queries = (size / 3) * 3;
        
        write(
            "T" + to_string(n) + "_push_random_then_pop",
            to_string(queries) + "\n"
        );
        
        // -- push --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + to_string(n) + "_push_random_then_pop",
                "1 " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- random --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + to_string(n) + "_push_random_then_pop",
                to_string(randint(0, 1)) + " " + to_string(randint(1, 1000000000)) + "\n"
            );
        }
        
        // -- pop --
        for (int i = 0; i < size / 3; i++) {
            write(
                "T" + to_string(n) + "_push_random_then_pop",
                "0 0\n"
            );
        }
        
        count_write = -1;
        write("T" + to_string(n) + "_push_random_then_pop", "");
    }
    
    // A_0 push + A_1 pop ...
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(pow(10, n));
        
        vector<int> blocks_size;
        
        int temporal_size = size;
        int max_block_size = size / (2 * n);
        while (temporal_size > 0) {
            int left = min(static_cast<int>(pow(size, 2.0/3.0)), temporal_size);
            int right = min(max_block_size, temporal_size);
            
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
            "T" + to_string(n) + "_push_then_pop",
            to_string(size) + "\n"
        );
        
        for (int i = 0; i < size; i++) {
            write(
                "T" + to_string(n) + "_push_then_pop",
                to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }
        
        int sum_blocks = 0;
        for (int block : blocks_size) {
            sum_blocks += block;
        }
        
        write(
            "T" + to_string(n) + "_push_then_pop",
            to_string(sum_blocks) + "\n"
        );
        
        for (int i = 0; i < static_cast<int>(blocks_size.size()); i++) {
            int block = blocks_size[i];
            
            if (i % 2 == 0) {
                // -- push --
                for (int j = 0; j < block; j++) {
                    write(
                        "T" + to_string(n) + "_push_then_pop",
                        "1 " + to_string(randint(1, 1000000000)) + "\n"
                    );
                }
            } else {
                // -- pop --
                for (int j = 0; j < block; j++) {
                    write(
                        "T" + to_string(n) + "_push_then_pop",
                        "0 0\n"
                    );
                }
            }
        }
        
        count_write = -1;
        write("T" + to_string(n) + "_push_then_pop", "");
    }
    
    return 0;
}


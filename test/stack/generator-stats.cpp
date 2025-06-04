#include <bits/stdc++.h>

using namespace std;

// Global variables
set<string> file_cleaned;
int count_write = 0;
string write_acumulation = "";
string CODE_DIR;

// Random number generation setup
random_device rd;
unsigned int seed = rd();
std::mt19937 gen(seed);
bool first_randint_call = true;

std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

void write_seed_to_file() {
    std::string filename = "seed" + get_current_date() + ".txt";
    std::ofstream seed_file(CODE_DIR + "/" + filename);
    seed_file << "Seed: " << seed << std::endl;
    seed_file.close();
}

int randint(int min, int max) {
    if (first_randint_call) {
        write_seed_to_file();
        first_randint_call = false;
    }
    
    std::uniform_int_distribution<> dis(min, max);
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

        vector<int> blocks;
        int queries = 0, temporal_size = size;
        for(int j = 0; j < 10; j++){
            int random_number = randint(max(size/10,1),size); 
            if(j % 2 == 0)
                temporal_size += random_number;
            else {
                random_number = min(random_number, temporal_size);
                temporal_size -= random_number;
            }
            blocks.push_back(random_number);
            queries += random_number;
        }

        write(
                "T" + to_string(n) + "_push_then_pop",
                to_string(queries) + "\n"
            );

        for(int j = 0; j < blocks.size(); j++){
            int block = blocks[j];
            for(int k = 0; k < block; k++){
                if(j % 2 == 0){
                    write(
                        "T" + to_string(n) + "_push_then_pop",
                        "1 " + to_string(randint(1, 1000000000)) + "\n"
                    );
                }else{
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


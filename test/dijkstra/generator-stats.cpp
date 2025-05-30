
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <random>
#include <filesystem>
#include <cmath>

// Global variables
std::string CODE_DIR;
std::set<std::string> file_cleaned;
int count_write = 0;
std::string write_acumulation = "";

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());

int randint(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void create_or_clear(const std::string& filename) {
    std::ofstream file(CODE_DIR + "/" + filename + ".in");
    file << "";
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
    // Get the directory of the current executable (equivalent to os.path.dirname(os.path.realpath(__file__)))
    CODE_DIR = std::filesystem::current_path().string();
    
    // Generate graphs with different densities
    for (int n = 1; n < 9; n++) {
        long long num_nodes = static_cast<long long>(std::pow(10, n));
        
        // Sparse graph (O(n) edges)
        long long edges_sparse = num_nodes * 2;
        write("T" + std::to_string(n) + "_sparse", std::to_string(num_nodes) + " " + std::to_string(edges_sparse) + "\n");
        
        std::set<std::pair<int, int>> edges_added;
        for (long long i = 0; i < edges_sparse; i++) {
            int u = randint(0, static_cast<int>(num_nodes - 1));
            int v = randint(0, static_cast<int>(num_nodes - 1));
            
            if (u == v) {
                v = (v + 1) % static_cast<int>(num_nodes);
            }
            
            edges_added.insert({u, v});
            edges_added.insert({v, u});
            int weight = randint(1, 1000);
            write("T" + std::to_string(n) + "_sparse", std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
        }
        
        count_write = -1;
        write("T" + std::to_string(n) + "_sparse", "");
        
        // Dense graph (O(n^2) edges, but maximum n*(n-1)/2)
        if (n <= 3) { // Only for small graphs to avoid huge test cases
            long long max_edges = (num_nodes * (num_nodes - 1)) / 2;
            long long edges_dense = std::min(num_nodes * num_nodes / 4, max_edges);
            write("T" + std::to_string(n) + "_dense", std::to_string(num_nodes) + " " + std::to_string(edges_dense) + "\n");
            
            edges_added.clear();
            for (long long i = 0; i < edges_dense; i++) {
                int u = randint(0, static_cast<int>(num_nodes - 1));
                int v = randint(0, static_cast<int>(num_nodes - 1));
                
                if (u == v) {
                    v = (v + 1) % static_cast<int>(num_nodes);
                }
                
                edges_added.insert({u, v});
                edges_added.insert({v, u});
                int weight = randint(1, 1000);
                write("T" + std::to_string(n) + "_dense", std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
            }
            
            count_write = -1;
            write("T" + std::to_string(n) + "_dense", "");
        }
    }
    
    return 0;
}


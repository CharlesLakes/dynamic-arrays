#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include <cmath> // For std::pow
#include <algorithm> // For std::min
#include <iomanip> // For std::put_time (used by generator_utils.hpp)
#include <chrono> // For std::chrono (used by generator_utils.hpp)

#include "../generator_utils.hpp" // Path relative to this file

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
    count_write %= 1000000; // Buffer up to 1,000,000 writes

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
 * @brief Generates a sparse graph.
 *
 * @param n The scale factor for the number of nodes (10^n nodes).
 * @param rand_gen Reference to the RandomGenerator instance.
 */
void generate_sparse_graph(int n_factor, RandomGenerator& rand_gen) {
    long long num_nodes = static_cast<long long>(std::pow(10, n_factor));
    long long edges_sparse = num_nodes * 2; // O(N) edges
    std::string graph_name = "T" + std::to_string(n_factor) + "_sparse";

    write(graph_name, std::to_string(num_nodes) + " " + std::to_string(edges_sparse) + "\n");

    std::set<std::pair<int, int>> edges_added;

    // Step 1: Connect all nodes in a linear chain (spanning tree)
    // This ensures the graph is connected.
    for (long long i = 0; i < num_nodes - 1; ++i) {
        int u = static_cast<int>(i);
        int v = static_cast<int>(i + 1);
        int weight = rand_gen.nextInt(1, 1000);
        edges_added.insert({u, v});
        edges_added.insert({v, u}); // For undirected graph representation in the set
        write(graph_name, std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
    }

    // Step 2: Add additional random edges to reach the target edge count
    // Avoids self-loops and duplicate edges.
    long long extra_edges = edges_sparse - (num_nodes - 1);
    while (extra_edges > 0) {
        int u = rand_gen.nextInt(0, static_cast<int>(num_nodes - 1));
        int v = rand_gen.nextInt(0, static_cast<int>(num_nodes - 1));

        if (u == v) { // No self-loops
            continue;
        }
        // Check if edge (u,v) or (v,u) already exists
        if (edges_added.count({u, v}) || edges_added.count({v, u})) {
            continue;
        }

        edges_added.insert({u, v});
        edges_added.insert({v, u});
        int weight = rand_gen.nextInt(1, 1000);
        write(graph_name, std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
        extra_edges--;
    }
    flush_write_buffer(graph_name); // Ensure all buffered content for this graph is written
}

/**
 * @brief Generates a dense graph.
 *
 * @param n The scale factor for the number of nodes (10^n nodes).
 * @param rand_gen Reference to the RandomGenerator instance.
 */
void generate_dense_graph(int n_factor, RandomGenerator& rand_gen) {
    long long num_nodes = static_cast<long long>(std::pow(10, n_factor));
    // O(N^2) edges, but cap at max possible edges N*(N-1)/2
    long long max_possible_edges = (num_nodes * (num_nodes - 1)) / 2;
    long long edges_dense = std::min(num_nodes * num_nodes / 4, max_possible_edges);
    // Ensure edges_dense does not exceed max_possible_edges, especially for small N
    if (num_nodes <= 1 && edges_dense > 0) edges_dense = 0; // No edges if 0 or 1 node
    if (num_nodes > 1 && edges_dense < num_nodes -1) edges_dense = num_nodes -1; // Ensure enough edges for a connected graph if possible


    std::string graph_name = "T" + std::to_string(n_factor) + "_dense";
    write(graph_name, std::to_string(num_nodes) + " " + std::to_string(edges_dense) + "\n");
    
    if (num_nodes == 0 || edges_dense == 0) { // No edges to generate
        flush_write_buffer(graph_name);
        return;
    }


    std::set<std::pair<int, int>> edges_added;

    // Step 1: Connect all nodes in a linear chain (spanning tree)
    // This ensures the graph is connected, if num_nodes > 1
    if (num_nodes > 1) {
        for (long long i = 0; i < num_nodes - 1; ++i) {
            int u = static_cast<int>(i);
            int v = static_cast<int>(i + 1);
            int weight = rand_gen.nextInt(1, 1000);
            edges_added.insert({u, v});
            edges_added.insert({v, u});
            write(graph_name, std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
        }
    }


    // Step 2: Add additional random edges to reach the target edge count
    long long current_edges = edges_added.size() / 2; // Each edge inserted twice in set
    long long extra_edges = edges_dense - current_edges;

    while (extra_edges > 0) {
        if (edges_added.size() / 2 >= max_possible_edges && num_nodes > 1) break; // Stop if all possible edges are added

        int u = rand_gen.nextInt(0, static_cast<int>(num_nodes - 1));
        int v = rand_gen.nextInt(0, static_cast<int>(num_nodes - 1));

        if (u == v) { // No self-loops
            continue;
        }
        if (edges_added.count({u, v}) || edges_added.count({v, u})) { // No duplicate edges
            continue;
        }

        edges_added.insert({u, v});
        edges_added.insert({v, u});
        int weight = rand_gen.nextInt(1, 1000);
        write(graph_name, std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(weight) + "\n");
        extra_edges--;
    }
    flush_write_buffer(graph_name); // Ensure all buffered content for this graph is written
}


int main(int argc, char* argv[]) {
    // Initialize random number generator using command line arguments
    RandomGenerator randomGenerator(argc, argv);

    // Determine the base directory for output files (usually the executable's directory)
    // Fallback to "." if current_path() fails or is empty.
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

    // Generate graphs with different densities
    // N goes from 10^1 to 10^8 nodes for sparse graphs
    // N goes from 10^1 to 10^4 nodes for dense graphs
    for (int n = 1; n < 9; ++n) { // n_factor from 1 to 8
        std::cout << "Generating sparse graph for N_factor = " << n << "..." << std::endl;
        generate_sparse_graph(n, randomGenerator);
        
        if (n <= 4) { // Only generate dense graphs for smaller N to avoid huge files/long times
            std::cout << "Generating dense graph for N_factor = " << n << "..." << std::endl;
            generate_dense_graph(n, randomGenerator);
        }
    }
    
    std::cout << "Graph generation complete." << std::endl;
    return 0;
}
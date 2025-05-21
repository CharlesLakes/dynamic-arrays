#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <deque>
#include "dijkstra.hpp"
#include "../debug.h"

using std::cin;
using std::cout;
using std::pair;
using std::priority_queue;
using std::vector;
using std::deque;
using std::greater;

// Define the type of priority queue to be used
using MinHeap = priority_queue<
    pair<int, int>,
    deque<pair<int, int>>>;

int main() {
    int num_nodes, num_edges;
    cin >> num_nodes >> num_edges;

    // Adjacency list: graph[u] = list of pairs (v, weight)
    vector<vector<pair<int, int>>> graph(num_nodes);

    // Read all edges
    for (int i = 0; i < num_edges; ++i) {
        int u, v, weight;
        cin >> u >> v >> weight;
        graph[u].emplace_back(v, weight);
        graph[v].emplace_back(u, weight); // For undirected graphs
    }

    // Run Dijkstra's algorithm from node 0
    vector<int> distances = dijkstra<MinHeap>(graph, 0);

    // Output the distances
    for (int cost : distances) {
        PRINT_INT(cost);
    }

    PRINT_END();
    PRINT_CHECK();

    return 0;
}
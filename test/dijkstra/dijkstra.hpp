#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <vector>
#include <queue>
#include <limits>
#include <functional>

// Use std namespace explicitly to avoid polluting the global scope
using std::greater;
using std::numeric_limits;
using std::pair;
using std::priority_queue;
using std::vector;

// Templated Dijkstra's algorithm to allow testing with different priority_queue containers
// Graph is represented as an adjacency list of pairs (neighbor, weight)
template <typename PriorityQueue = priority_queue<
              pair<int, int>,
              vector<pair<int, int>>,
              greater<pair<int, int>>>>
vector<int> dijkstra(const vector<vector<pair<int, int>>> &graph, int source)
{
    using pii = pair<int, int>; // (distance, node)

    // Initialize distances to infinity
    vector<int> distance(graph.size(), numeric_limits<int>::max());

    // Priority queue to select the node with the smallest distance
    PriorityQueue pq;

    // Distance to source is 0
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        auto [dist_u, u] = pq.top();
        pq.pop();

        // If the distance is outdated, skip
        if (dist_u != distance[u])
            continue;

        // Relax all adjacent edges
        for (const auto &[v, weight] : graph[u])
        {
            if (dist_u + weight < distance[v])
            {
                distance[v] = dist_u + weight;
                pq.push({distance[v], v});
            }
        }
    }

    return distance;
}

#endif

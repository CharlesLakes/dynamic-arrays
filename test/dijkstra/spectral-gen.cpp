/*
This code is not my own. It was provided by Roberto Asín Acha as a test case generator.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <memory>
#include <functional>
#include <queue>
#include <numeric>

// Type definitions for clarity
using Edge = std::pair<int, int>;
using EdgeList = std::vector<Edge>;
using VertexCoord = std::vector<double>;
using VertexList = std::vector<VertexCoord>;
using AdjacencyList = std::vector<std::vector<int>>;
using DegreeVector = std::vector<double>;

// Forward declarations
class DisjointSet;
EdgeList ensureConnectedGraph(const VertexList& vertices, const EdgeList& edges);
std::tuple<std::vector<int>, std::vector<int>, double, std::vector<double>, double, int, int, int> 
findOptimalThresholdBisection(const std::vector<double>& fiedlerVector, const AdjacencyList& adjList, const DegreeVector& degrees);

// DisjointSet for connectivity checking
class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int numComponents;

public:
    DisjointSet(int n) : parent(n), rank(n, 0), numComponents(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }

    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) {
            return false;  // Already in same set
        }
        
        // Union by rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        
        numComponents--;
        return true;
    }
    
    int getNumComponents() const {
        return numComponents;
    }
};

// Correct implementation of Random Geometric Graph generation
VertexList generateRandomVertices(int numVertices, int dimensions, double minCoord = 0.0, double maxCoord = 1.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(minCoord, maxCoord);
    
    VertexList vertices(numVertices, VertexCoord(dimensions));
    for (int i = 0; i < numVertices; ++i) {
        for (int d = 0; d < dimensions; ++d) {
            vertices[i][d] = dist(gen);
        }
    }
    return vertices;
}

// Calculate Euclidean distance between two vertices
double calcDistance(const VertexCoord& a, const VertexCoord& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

// Completely rewritten Random Geometric Graph generator
std::pair<VertexList, EdgeList> generateRandomGeometricGraph(int numVertices, double avgDegree, 
                                                          int dimensions = 2,
                                                          double minCoord = 0.0, 
                                                          double maxCoord = 1.0, 
                                                          bool ensureConnected = true) {
    // Step 1: Generate random vertex positions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(minCoord, maxCoord);
    
    VertexList vertices(numVertices, VertexCoord(dimensions));
    for (int i = 0; i < numVertices; ++i) {
        for (int d = 0; d < dimensions; ++d) {
            vertices[i][d] = dist(gen);
        }
    }
    
    // Step 2: Determine connection radius based on average degree
    // For a random geometric graph in [0,1]^d with n vertices and average degree k:
    // In 2D: r_2D ≈ sqrt(k/(πn))
    // General case: r_d ≈ (k/((n-1)c_d))^(1/d) where c_d is the volume of unit d-ball
    
    double radius = 0.0;
    double scaledVolume = 1.0;
    
    // For 2D (most common case, exact formula)
    if (dimensions == 2) {
        radius = std::sqrt(avgDegree / (M_PI * numVertices));
        std::cout << "2D Random Geometric Graph: radius = " << radius << std::endl;
    }
    else {
        // General approximation for d dimensions using gamma function
        double unitBallVolume = std::pow(M_PI, dimensions/2.0) / 
                               std::tgamma(dimensions/2.0 + 1.0);
        radius = std::pow(avgDegree / (unitBallVolume * numVertices), 1.0/dimensions);
        std::cout << dimensions << "D Random Geometric Graph: radius = " << radius << std::endl;
    }
    
    // Scale radius by domain size
    double domainSize = maxCoord - minCoord;
    radius *= domainSize;
    
    // Step 3: Create edges based strictly on geometric distance
    EdgeList edges;
    edges.reserve(numVertices * avgDegree); // Pre-allocate memory
    
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            // Compute squared Euclidean distance (faster than taking sqrt)
            double sqDist = 0.0;
            for (int d = 0; d < dimensions; ++d) {
                double diff = vertices[i][d] - vertices[j][d];
                sqDist += diff * diff;
            }
            
            // Connect if distance is within radius
            if (sqDist <= radius * radius) {
                edges.emplace_back(i + 1, j + 1);  // 1-indexed for DIMACS
            }
        }
    }
    
    // Report actual average degree
    double actualAvgDegree = 2.0 * edges.size() / numVertices;
    std::cout << "Created Random Geometric Graph with " << numVertices << " vertices and " 
              << edges.size() << " edges" << std::endl;
    std::cout << "Average degree: " << actualAvgDegree << " (target: " << avgDegree << ")" << std::endl;
    
    // Step 4: If connectivity is required, add minimal edges to ensure connectivity
    if (ensureConnected) {
        // Check if the graph is already connected
        DisjointSet ds(numVertices);
        for (const auto& [u, v] : edges) {
            ds.unionSets(u-1, v-1);  // Convert to 0-indexed
        }
        
        if (ds.getNumComponents() == 1) {
            std::cout << "Graph is already connected." << std::endl;
        }
        else {
            std::cout << "Graph has " << ds.getNumComponents() << " components. Adding minimal connecting edges..." << std::endl;
            
            // Find component for each vertex
            std::vector<int> componentOf(numVertices);
            for (int i = 0; i < numVertices; ++i) {
                componentOf[i] = ds.find(i);
            }
            
            // Group vertices by component
            std::unordered_map<int, std::vector<int>> componentVertices;
            for (int i = 0; i < numVertices; ++i) {
                componentVertices[componentOf[i]].push_back(i);
            }
            
            // Connect components greedily
            std::set<int> connectedComponents;
            connectedComponents.insert(componentOf[0]);  // Start with first component
            
            while (connectedComponents.size() < ds.getNumComponents()) {
                double minDist = std::numeric_limits<double>::max();
                std::pair<int, int> bestEdge;
                
                // For each connected component
                for (int comp1 : connectedComponents) {
                    // Find closest vertex in an unconnected component
                    for (int comp2 = 0; comp2 < numVertices; ++comp2) {
                        if (connectedComponents.count(componentOf[comp2]) > 0) continue; // Skip already connected
                        
                        for (int v1 : componentVertices[comp1]) {
                            for (int v2 : componentVertices[componentOf[comp2]]) {
                                // Calculate distance
                                double sqDist = 0.0;
                                for (int d = 0; d < dimensions; ++d) {
                                    double diff = vertices[v1][d] - vertices[v2][d];
                                    sqDist += diff * diff;
                                }
                                
                                if (sqDist < minDist) {
                                    minDist = sqDist;
                                    bestEdge = {v1, v2};
                                }
                            }
                        }
                    }
                }
                
                // Add the best edge
                edges.emplace_back(bestEdge.first + 1, bestEdge.second + 1);  // 1-indexed
                
                // Union the components
                int comp1 = componentOf[bestEdge.first];
                int comp2 = componentOf[bestEdge.second];
                
                // Mark this component as connected
                connectedComponents.insert(comp2);
            }
            
            // Report updated statistics
            double newAvgDegree = 2.0 * edges.size() / numVertices;
            std::cout << "After ensuring connectivity: " << edges.size() << " edges, "
                     << "average degree: " << newAvgDegree << std::endl;
        }
    }
    
    return {vertices, edges};
}

// Much more efficient implementation for ensuring graph connectivity
EdgeList ensureConnectedGraph(const VertexList& vertices, const EdgeList& edges) {
    int n = vertices.size();
    if (n <= 1) {
        return edges;
    }
    
    // Create disjoint-set
    DisjointSet ds(n);
    
    // Create a set for quick edge existence check
    std::unordered_set<uint64_t> edgeSet;
    for (const auto& [u, v] : edges) {
        // Create a unique key for each edge (independent of order)
        uint64_t minV = std::min(u, v);
        uint64_t maxV = std::max(u, v);
        uint64_t key = (static_cast<uint64_t>(minV) << 32) | maxV;
        edgeSet.insert(key);
        
        // Add to disjoint-set
        ds.unionSets(u-1, v-1);  // Convert to 0-indexed
    }
    
    // Already connected?
    if (ds.getNumComponents() == 1) {
        std::cout << "Graph is already connected." << std::endl;
        return edges;
    }
    
    std::cout << "Graph has " << ds.getNumComponents() 
              << " disconnected components. Adding edges to connect them..." << std::endl;
    
    // Find representative vertices for each component
    std::vector<std::vector<int>> components(n);
    for (int i = 0; i < n; ++i) {
        int root = ds.find(i);
        components[root].push_back(i);
    }
    
    // Remove empty components
    components.erase(
        std::remove_if(components.begin(), components.end(), 
                      [](const auto& comp) { return comp.empty(); }),
        components.end()
    );
    
    std::cout << "Finding minimum spanning forest to connect components..." << std::endl;
    
    // Create MST-like structure between components (more efficient than all-pairs)
    EdgeList newEdges = edges;
    int componentsConnected = 0;
    const int numComponents = components.size();
    
    // Use priority queue for efficient "closest edge" finding
    using Edge = std::tuple<double, int, int, int, int>; // (dist, comp1, comp2, v1, v2)
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    
    // Find closest vertices between the first component and all others
    for (int c2 = 1; c2 < numComponents; ++c2) {
        double minDist = std::numeric_limits<double>::infinity();
        int minV1 = -1, minV2 = -1;
        
        // Sample approach: only check a subset of vertices between components
        const int maxSamplesPerComponent = 100; // Limit checks to avoid O(n²)
        
        for (int i = 0; i < std::min(maxSamplesPerComponent, static_cast<int>(components[0].size())); ++i) {
            int v1Idx = components[0][i % components[0].size()];
            
            for (int j = 0; j < std::min(maxSamplesPerComponent, static_cast<int>(components[c2].size())); ++j) {
                int v2Idx = components[c2][j % components[c2].size()];
                
                // 1-indexed vertices
                int v1 = v1Idx + 1;
                int v2 = v2Idx + 1;
                
                // Skip if edge already exists
                uint64_t minV = std::min(v1, v2);
                uint64_t maxV = std::max(v1, v2);
                uint64_t key = (static_cast<uint64_t>(minV) << 32) | maxV;
                if (edgeSet.count(key) > 0) continue;
                
                double dist = calcDistance(vertices[v1Idx], vertices[v2Idx]);
                if (dist < minDist) {
                    minDist = dist;
                    minV1 = v1;
                    minV2 = v2;
                }
            }
        }
        
        if (minV1 != -1 && minV2 != -1) {
            pq.emplace(minDist, 0, c2, minV1, minV2);
        }
    }
    
    // Use a minimum spanning tree approach to connect components
    std::unordered_set<int> connectedComponents;
    connectedComponents.insert(0); // Start with first component
    
    while (componentsConnected < numComponents - 1 && !pq.empty()) {
        auto [dist, c1, c2, v1, v2] = pq.top();
        pq.pop();
        
        if (connectedComponents.count(c2) > 0) continue; // Already connected
        
        // Add the edge to connect these components
        newEdges.emplace_back(v1, v2);
        componentsConnected++;
        connectedComponents.insert(c2);
        
        // Find edges to newly reachable component
        if (componentsConnected < numComponents - 1) {
            for (int nextC = 0; nextC < numComponents; ++nextC) {
                if (connectedComponents.count(nextC) > 0) continue; // Skip already connected
                
                double minDist = std::numeric_limits<double>::infinity();
                int minV1 = -1, minV2 = -1;
                int sourceComp = -1;
                
                // Try connecting from any connected component to this unconnected one
                const int maxSamplesPerComponent = 50; // Further reduce samples
                
                for (int connectedComp : connectedComponents) {
                    for (int i = 0; i < std::min(maxSamplesPerComponent, static_cast<int>(components[connectedComp].size())); ++i) {
                        int v1Idx = components[connectedComp][i % components[connectedComp].size()];
                        
                        for (int j = 0; j < std::min(maxSamplesPerComponent, static_cast<int>(components[nextC].size())); ++j) {
                            int v2Idx = components[nextC][j % components[nextC].size()];
                            
                            int v1 = v1Idx + 1;
                            int v2 = v2Idx + 1;
                            
                            // Skip if edge already exists
                            uint64_t minV = std::min(v1, v2);
                            uint64_t maxV = std::max(v1, v2);
                            uint64_t key = (static_cast<uint64_t>(minV) << 32) | maxV;
                            if (edgeSet.count(key) > 0) continue;
                            
                            double dist = calcDistance(vertices[v1Idx], vertices[v2Idx]);
                            if (dist < minDist) {
                                minDist = dist;
                                minV1 = v1;
                                minV2 = v2;
                                sourceComp = connectedComp;
                            }
                        }
                    }
                }
                
                if (minV1 != -1 && minV2 != -1) {
                    pq.emplace(minDist, sourceComp, nextC, minV1, minV2);
                }
            }
        }
    }
    
    std::cout << "Added " << componentsConnected << " edges to ensure connectivity." << std::endl;
    return newEdges;
}

// Generate a grid graph
EdgeList generateGridGraph(int rows, int cols, bool connectDiagonals = false) {
    VertexList vertices;
    EdgeList edges;
    
    // Generate vertices
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            vertices.push_back({static_cast<double>(r), static_cast<double>(c)});
        }
    }
    
    // Generate edges
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int v = r * cols + c + 1;  // 1-indexed
            
            // Connect to right neighbor
            if (c < cols - 1) {
                edges.emplace_back(v, v + 1);
            }
            
            // Connect to bottom neighbor
            if (r < rows - 1) {
                edges.emplace_back(v, v + cols);
            }
            
            // Connect diagonals if requested
            if (connectDiagonals) {
                // Bottom-right
                if (r < rows - 1 && c < cols - 1) {
                    edges.emplace_back(v, v + cols + 1);
                }
                
                // Bottom-left
                if (r < rows - 1 && c > 0) {
                    edges.emplace_back(v, v + cols - 1);
                }
            }
        }
    }
    
    return edges;
}

// Generate a circle graph
EdgeList generateCircleGraph(int numVertices, double radius = 1.0, int connectNeighbors = 1) {
    VertexList vertices;
    EdgeList edges;
    
    // Generate vertices on a circle
    for (int i = 0; i < numVertices; ++i) {
        double angle = 2.0 * M_PI * i / numVertices;
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        vertices.push_back({x, y});
    }
    
    // Connect neighbors
    for (int i = 0; i < numVertices; ++i) {
        for (int k = 1; k <= connectNeighbors; ++k) {
            int j = (i + k) % numVertices;
            edges.emplace_back(i + 1, j + 1);  // 1-indexed
        }
    }
    
    return edges;
}

// Save graph in DIMACS format
void saveDimacsFormat(const VertexList& vertices, const EdgeList& edges, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    
    // Write header
    int dimensions = vertices.empty() ? 0 : vertices[0].size();
    double avgDegree = vertices.empty() ? 0 : 2.0 * edges.size() / vertices.size();
    
    file << "c Geometric Graph\n";
    file << "c Generated with " << vertices.size() << " vertices in " 
         << dimensions << "-dimensional space\n";
    file << "c Average degree: " << avgDegree << "\n";
    
    // Write problem line
    file << "p edge " << vertices.size() << " " << edges.size() << "\n";
    
    // Write vertex coordinates as comments
    for (size_t i = 0; i < vertices.size(); ++i) {
        file << "c v " << (i + 1);
        for (double coord : vertices[i]) {
            file << " " << std::fixed << std::setprecision(6) << coord;
        }
        file << "\n";
    }
    
    // Write edges
    for (const auto& [u, v] : edges) {
        file << "e " << u << " " << v << "\n";
    }
    
    std::cout << "Graph saved to " << filename << std::endl;
}

// Read a graph in DIMACS format
std::pair<VertexList, EdgeList> readDimacsFormat(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return {{}, {}};
    }
    
    VertexList vertices;
    EdgeList edges;
    int numVertices = 0;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        if (line.compare(0, 4, "c v ") == 0) {
            // Parse vertex coordinates
            std::istringstream iss(line.substr(4));
            int id;
            iss >> id;
            
            // Ensure vertices vector is large enough
            if (id > static_cast<int>(vertices.size())) {
                vertices.resize(id);
            }
            
            // Read coordinates
            VertexCoord coords;
            double coord;
            while (iss >> coord) {
                coords.push_back(coord);
            }
            
            vertices[id-1] = coords;
        }
        else if (line.compare(0, 7, "p edge ") == 0) {
            // Parse problem line
            std::istringstream iss(line.substr(7));
            iss >> numVertices;
            // Ignore num edges
        }
        else if (line[0] == 'e') {
            // Parse edge
            std::istringstream iss(line.substr(2));
            int u, v;
            if (iss >> u >> v) {
                edges.emplace_back(u, v);
            }
        }
    }
    
    // If no vertices found, generate random positions
    if (vertices.empty() && numVertices > 0) {
        std::cout << "Warning: No vertex coordinates found in file. Generating random positions." << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        vertices.resize(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            vertices[i] = {dist(gen), dist(gen)};
        }
    }
    
    return {vertices, edges};
}

// Save a cut to a file
void saveCutToFile(const std::vector<int>& partitionA, const std::vector<int>& partitionB,
                  double threshold, double cheeger, int cuts, int volA, int volB,
                  const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    
    // Write metadata
    file << "c Cut metadata\n";
    file << "c threshold=" << std::fixed << std::setprecision(6) << threshold 
         << ", cheeger=" << cheeger << ", cut_size=" << cuts << "\n";
    file << "c volumes: A=" << volA << ", B=" << volB << "\n";
    file << "c partition sizes: A=" << partitionA.size() << ", B=" << partitionB.size() << "\n";
    
    // Write partition info
    file << "p cut " << partitionA.size() << " " << partitionB.size() << " " << cuts << "\n";
    
    // Write partition A vertices
    file << "a";
    for (int v : partitionA) {
        file << " " << v;
    }
    file << "\n";
    
    // Write partition B vertices
    file << "b";
    for (int v : partitionB) {
        file << " " << v;
    }
    file << "\n";
    
    // Write crossing edges count
    file << "c crossing edges: " << cuts << "\n";
}

// More efficient adjacency list building with reserve
AdjacencyList buildAdjacencyList(int numVertices, const EdgeList& edges) {
    AdjacencyList adjList(numVertices);
    
    // Pre-allocate space based on average degree
    double avgDegree = edges.size() * 2.0 / numVertices;
    for (int i = 0; i < numVertices; ++i) {
        adjList[i].reserve(avgDegree * 1.5); // Add some buffer
    }
    
    for (const auto& [u, v] : edges) {
        int u0 = u - 1;
        int v0 = v - 1;
        adjList[u0].push_back(v0);
        adjList[v0].push_back(u0);
    }
    
    return adjList;
}

// Compute vertex degrees
DegreeVector computeDegrees(const AdjacencyList& adjList) {
    DegreeVector degrees(adjList.size());
    for (size_t i = 0; i < adjList.size(); ++i) {
        degrees[i] = adjList[i].size();
    }
    return degrees;
}

/// Lanczos algorithm for computing the Fiedler vector
std::vector<double> computeFiedlerVectorLanczos(const AdjacencyList& adjList, const DegreeVector& degrees) {
    int n = adjList.size();
    
    // Maximum iterations and orthogonalization parameters
    const int maxIter = std::min(n, 300);  // At most n iterations, but cap at 300
    const double reorthogonalizationTol = 1e-8;
    
    // Create normalized Laplacian matrix-vector multiplication function
    auto matVecMultiply = [&](const std::vector<double>& v) {
        std::vector<double> result(n, 0.0);
        
        // L*v where L is the normalized Laplacian
        for (int i = 0; i < n; ++i) {
            // Diagonal term: I*v
            result[i] = v[i];
            
            // Off-diagonal term: -D^(-1/2) A D^(-1/2) * v
            if (degrees[i] > 0) {
                double di_sqrt_inv = 1.0 / std::sqrt(degrees[i]);
                for (int j : adjList[i]) {
                    if (degrees[j] > 0) {
                        double dj_sqrt_inv = 1.0 / std::sqrt(degrees[j]);
                        result[i] -= di_sqrt_inv * dj_sqrt_inv * v[j];
                    }
                }
            }
        }
        
        return result;
    };
    
    // Generate random initial vector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);
    
    std::vector<double> q0(n, 0.0);  // Zero vector (not used in iteration)
    std::vector<double> q1(n);       // Initial vector
    
    // Generate first basis vector and normalize
    for (int i = 0; i < n; ++i) {
        q1[i] = dist(gen);
    }
    
    // Make orthogonal to constant vector (nullspace of L)
    double degSum = 0.0;
    for (double d : degrees) {
        degSum += d;
    }
    
    std::vector<double> constVector(n);
    for (int i = 0; i < n; ++i) {
        constVector[i] = std::sqrt(degrees[i] / degSum);
    }
    
    double dotProd = 0.0;
    for (int i = 0; i < n; ++i) {
        dotProd += q1[i] * constVector[i];
    }
    
    for (int i = 0; i < n; ++i) {
        q1[i] -= dotProd * constVector[i];
    }
    
    // Normalize q1
    double norm = 0.0;
    for (int i = 0; i < n; ++i) {
        norm += q1[i] * q1[i];
    }
    norm = std::sqrt(norm);
    
    for (int i = 0; i < n; ++i) {
        q1[i] /= norm;
    }
    
    // Tridiagonal matrix elements
    std::vector<double> alpha(maxIter, 0.0);  // Diagonal
    std::vector<double> beta(maxIter, 0.0);   // Off-diagonal
    
    // Store basis vectors (needed for full reorthogonalization)
    std::vector<std::vector<double>> Q;
    Q.push_back(std::vector<double>(n, 0.0));  // q0 (unused)
    Q.push_back(q1);
    
    // Lanczos iteration
    std::vector<double> r = matVecMultiply(q1);
    
    alpha[0] = 0.0;
    for (int i = 0; i < n; ++i) {
        alpha[0] += q1[i] * r[i];
    }
    
    // r = r - alpha[0] * q1
    for (int i = 0; i < n; ++i) {
        r[i] -= alpha[0] * q1[i];
    }
    
    // Compute full reorthogonalization against all previous vectors
    for (size_t j = 0; j <= 1; ++j) {
        double dotProd = 0.0;
        for (int i = 0; i < n; ++i) {
            dotProd += r[i] * Q[j][i];
        }
        
        for (int i = 0; i < n; ++i) {
            r[i] -= dotProd * Q[j][i];
        }
    }
    
    // Main Lanczos iteration
    for (int j = 1; j < maxIter; ++j) {
        beta[j-1] = 0.0;
        for (int i = 0; i < n; ++i) {
            beta[j-1] += r[i] * r[i];
        }
        beta[j-1] = std::sqrt(beta[j-1]);
        
        // Check for convergence or breakdown
        if (beta[j-1] < 1e-10) {
            std::cout << "Lanczos breakdown at iteration " << j << std::endl;
            break;
        }
        
        // Compute next basis vector
        std::vector<double> qNext(n);
        for (int i = 0; i < n; ++i) {
            qNext[i] = r[i] / beta[j-1];
        }
        
        Q.push_back(qNext);
        
        // Compute r = A*qNext - beta[j-1]*Q[j]
        r = matVecMultiply(qNext);
        
        alpha[j] = 0.0;
        for (int i = 0; i < n; ++i) {
            alpha[j] += qNext[i] * r[i];
        }
        
        for (int i = 0; i < n; ++i) {
            r[i] = r[i] - alpha[j] * qNext[i] - beta[j-1] * Q[j][i];
        }
        
        // Full reorthogonalization against all previous vectors
        for (size_t k = 0; k <= j+1; ++k) {
            double dotProd = 0.0;
            for (int i = 0; i < n; ++i) {
                dotProd += r[i] * Q[k][i];
            }
            
            if (std::abs(dotProd) > reorthogonalizationTol) {
                for (int i = 0; i < n; ++i) {
                    r[i] -= dotProd * Q[k][i];
                }
            }
        }
        
        // Break if we've done at least 10 iterations and beta is small
        if (j >= 10 && beta[j-1] < 1e-8) {
            std::cout << "Lanczos converged at iteration " << j << std::endl;
            break;
        }
    }
    
    // Determine actual iterations performed
    int actualIters = 0;
    for (int j = 0; j < maxIter; ++j) {
        if (beta[j] != 0.0 || j == 0) {
            actualIters = j + 1;
        } else {
            break;
        }
    }
    
    // Construct tridiagonal matrix
    int m = actualIters;
    std::vector<std::vector<double>> T(m, std::vector<double>(m, 0.0));
    
    for (int i = 0; i < m; ++i) {
        T[i][i] = alpha[i];
        if (i > 0) {
            T[i][i-1] = beta[i-1];
        }
        if (i < m-1) {
            T[i][i+1] = beta[i];
        }
    }
    
    // Find eigenvalues/vectors of the tridiagonal matrix
    // For simplicity, we'll just find them all (the matrix is small)
    std::vector<double> eigenvalues(m);
    std::vector<std::vector<double>> eigenvectors(m, std::vector<double>(m));
    
    // Use LAPACK-style implicit QR algorithm for tridiagonal matrix
    // For simplicity, a basic implementation:
    
    // Start with identity matrix for eigenvectors
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            eigenvectors[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    // Perform QR iterations
    const int maxQrIters = 1000;
    bool converged = false;
    
    for (int iter = 0; iter < maxQrIters && !converged; ++iter) {
        // One QR step with implicit shift
        converged = true;
        
        for (int i = 0; i < m-1; ++i) {
            double c, s;
            // Get Givens rotation
            if (std::abs(T[i][i] - T[i+1][i+1]) < 1e-10) {
                c = 1.0 / std::sqrt(2.0);
                s = 1.0 / std::sqrt(2.0);
            } else {
                double tau = (T[i+1][i+1] - T[i][i]) / (2.0 * T[i][i+1]);
                double t = 1.0 / (std::abs(tau) + std::sqrt(1.0 + tau*tau));
                if (tau < 0.0) t = -t;
                c = 1.0 / std::sqrt(1.0 + t*t);
                s = t * c;
            }
            
            // Apply rotation to T
            for (int j = std::max(0, i-1); j < m; ++j) {
                double temp1 = T[i][j];
                double temp2 = (j <= i+1) ? T[i+1][j] : 0.0;
                T[i][j] = c * temp1 + s * temp2;
                if (j <= i+1) {
                    T[i+1][j] = -s * temp1 + c * temp2;
                }
            }
            
            for (int j = 0; j <= std::min(m-1, i+2); ++j) {
                double temp1 = (j <= i) ? T[j][i] : 0.0;
                double temp2 = T[j][i+1];
                if (j <= i) {
                    T[j][i] = c * temp1 + s * temp2;
                }
                T[j][i+1] = -s * temp1 + c * temp2;
            }
            
            // Apply rotation to eigenvectors
            for (int j = 0; j < m; ++j) {
                double temp1 = eigenvectors[j][i];
                double temp2 = eigenvectors[j][i+1];
                eigenvectors[j][i] = c * temp1 + s * temp2;
                eigenvectors[j][i+1] = -s * temp1 + c * temp2;
            }
            
            // Check if off-diagonal elements are small
            if (i < m-2 && std::abs(T[i+1][i+2]) > 1e-10) {
                converged = false;
            }
        }
    }
    
    // Extract eigenvalues from diagonal
    for (int i = 0; i < m; ++i) {
        eigenvalues[i] = T[i][i];
    }
    
    // Find the second smallest eigenvalue (Fiedler)
    // First, we need to sort them
    std::vector<int> indices(m);
    std::iota(indices.begin(), indices.end(), 0);
    
    std::sort(indices.begin(), indices.end(), 
              [&eigenvalues](int a, int b) { return eigenvalues[a] < eigenvalues[b]; });
    
    // The second smallest is at index 1 (after sorting)
    int fiedlerIndex = indices[1];
    
    // Reconstruct the Fiedler vector in the original basis
    std::vector<double> fiedlerVector(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            fiedlerVector[i] += eigenvectors[fiedlerIndex][j] * Q[j+1][i];
        }
    }
    
    // Normalize
    norm = 0.0;
    for (int i = 0; i < n; ++i) {
        norm += fiedlerVector[i] * fiedlerVector[i];
    }
    norm = std::sqrt(norm);
    
    for (int i = 0; i < n; ++i) {
        fiedlerVector[i] /= norm;
    }
    
    std::cout << "Fiedler eigenvalue: " << eigenvalues[fiedlerIndex] << std::endl;
    
    return fiedlerVector;
}

// Corrected Cheeger constant calculation
std::tuple<double, int, int, int> calculateCheegerConstant(
    const std::vector<int>& partitionA, const std::vector<int>& partitionB,
    const AdjacencyList& adjList, const DegreeVector& degrees) {
    
    // Convert to 0-indexed sets for fast lookup
    std::unordered_set<int> setA;
    for (int v : partitionA) {
        setA.insert(v - 1);  // Convert to 0-indexed
    }
    
    std::unordered_set<int> setB;
    for (int v : partitionB) {
        setB.insert(v - 1);  // Convert to 0-indexed
    }
    
    // Build an edge set to avoid double counting
    std::set<std::pair<int, int>> edgesAcrossCut;
    
    for (int u : setA) {
        for (int v : adjList[u]) {
            if (setB.count(v) > 0) {
                // Ensure we count each edge only once by ordering vertices
                int minV = std::min(u, v);
                int maxV = std::max(u, v);
                edgesAcrossCut.insert({minV, maxV});
            }
        }
    }
    
    int crossingEdges = edgesAcrossCut.size();
    
    // Calculate volumes (sum of degrees)
    int volA = 0;
    for (int u : setA) {
        volA += degrees[u];
    }
    
    int volB = 0;
    for (int u : setB) {
        volB += degrees[u];
    }
    
    // Sanity check
    int totalVolume = volA + volB;
    int expectedTotalVolume = 0;
    for (const auto& d : degrees) {
        expectedTotalVolume += d;
    }
    
    if (std::abs(totalVolume - expectedTotalVolume) > 1e-6) {
        std::cout << "WARNING: Volume mismatch! " << totalVolume 
                  << " vs expected " << expectedTotalVolume << std::endl;
    }
    
    // Calculate Cheeger constant phi(G) = |E(S,S̄)|/min(vol(S),vol(S̄))
    int minVolume = std::min(volA, volB);
    double cheegerConstant = minVolume > 0 ? static_cast<double>(crossingEdges) / minVolume : 
                                           std::numeric_limits<double>::infinity();
    
    return {cheegerConstant, crossingEdges, volA, volB};
}

// Find optimal threshold for bisection
std::tuple<std::vector<int>, std::vector<int>, double, std::vector<double>, double, int, int, int> 
findOptimalThresholdBisection(const std::vector<double>& fiedlerVector, const AdjacencyList& adjList, const DegreeVector& degrees) {
    // Get unique values from Fiedler vector
    std::vector<double> uniqueValues = fiedlerVector;
    std::sort(uniqueValues.begin(), uniqueValues.end());
    auto last = std::unique(uniqueValues.begin(), uniqueValues.end());
    uniqueValues.erase(last, uniqueValues.end());
    
    // If too many values, sample them
    std::vector<double> thresholdCandidates;
    if (uniqueValues.size() > 50) {
        int step = std::max(1, static_cast<int>(uniqueValues.size() / 50));
        for (size_t i = 0; i < uniqueValues.size(); i += step) {
            thresholdCandidates.push_back(uniqueValues[i]);
        }
        
        // Always include zero if in range
        if (uniqueValues.front() <= 0 && uniqueValues.back() >= 0) {
            auto it = std::find(thresholdCandidates.begin(), thresholdCandidates.end(), 0.0);
            if (it == thresholdCandidates.end()) {
                thresholdCandidates.push_back(0.0);
                std::sort(thresholdCandidates.begin(), thresholdCandidates.end());
            }
        }
    } else {
        thresholdCandidates = uniqueValues;
    }
    
    // Add midpoints between thresholds
    std::vector<double> finalThresholds;
    finalThresholds.reserve(2 * thresholdCandidates.size());
    
    for (size_t i = 0; i < thresholdCandidates.size() - 1; ++i) {
        finalThresholds.push_back(thresholdCandidates[i]);
        // Add midpoint
        finalThresholds.push_back((thresholdCandidates[i] + thresholdCandidates[i+1]) / 2.0);
    }
    
    // Add last value
    if (!thresholdCandidates.empty()) {
        finalThresholds.push_back(thresholdCandidates.back());
    }
    
    // Ensure zero is included if in range
    if (!uniqueValues.empty() && uniqueValues.front() <= 0 && uniqueValues.back() >= 0) {
        auto it = std::find(finalThresholds.begin(), finalThresholds.end(), 0.0);
        if (it == finalThresholds.end()) {
            finalThresholds.push_back(0.0);
            std::sort(finalThresholds.begin(), finalThresholds.end());
        }
    }
    
    // Find best threshold
    double bestThreshold = 0.0;
    std::vector<int> bestPartitionA;
    std::vector<int> bestPartitionB;
    double bestCheeger = std::numeric_limits<double>::infinity();
    int bestCuts = 0;
    int bestVolA = 0;
    int bestVolB = 0;
    
    std::cout << "Testing " << finalThresholds.size() << " threshold values for optimal bisection..." << std::endl;
    
    // Track all results
    struct Result {
        double threshold;
        double cheeger;
        int cuts;
        int volA;
        int volB;
        int sizeA;
        int sizeB;
        std::vector<int> partA;
        std::vector<int> partB;
    };
    
    std::vector<Result> allResults;
    allResults.reserve(finalThresholds.size());
    
    for (double threshold : finalThresholds) {
        // Create partitions
        std::vector<int> partitionA;
        std::vector<int> partitionB;
        
        for (size_t i = 0; i < fiedlerVector.size(); ++i) {
            if (fiedlerVector[i] >= threshold) {
                partitionA.push_back(i + 1);  // 1-indexed
            } else {
                partitionB.push_back(i + 1);  // 1-indexed
            }
        }
        
        // Skip if either partition is empty
        if (partitionA.empty() || partitionB.empty()) {
            continue;
        }
        
        // Calculate Cheeger constant
        auto [cheeger, cuts, volA, volB] = calculateCheegerConstant(
            partitionA, partitionB, adjList, degrees);
        
        // Store result
        allResults.push_back({
            threshold, cheeger, cuts, volA, volB, 
            static_cast<int>(partitionA.size()), 
            static_cast<int>(partitionB.size()),
            partitionA, partitionB
        });
        
        // Update best if better
        if (cheeger < bestCheeger) {
            bestCheeger = cheeger;
            bestThreshold = threshold;
            bestPartitionA = std::move(partitionA);
            bestPartitionB = std::move(partitionB);
            bestCuts = cuts;
            bestVolA = volA;
            bestVolB = volB;
        }
    }
    
    // Sort results for reporting
    std::sort(allResults.begin(), allResults.end(), 
              [](const Result& a, const Result& b) { return a.cheeger < b.cheeger; });
    
    // Print results table
    std::cout << "\nAll tested thresholds and their Cheeger constants:" << std::endl;
    std::cout << std::setw(12) << "Threshold" << " | " 
              << std::setw(10) << "Cheeger" << " | " 
              << std::setw(10) << "Cut Size" << " | " 
              << std::setw(10) << "Vol A" << " | " 
              << std::setw(10) << "Vol B" << " | " 
              << std::setw(6) << "|A|" << " | " 
              << std::setw(6) << "|B|" << " | " 
              << "Notes" << std::endl;
    
    std::cout << std::string(12, '-') << " | " 
              << std::string(10, '-') << " | " 
              << std::string(10, '-') << " | " 
              << std::string(10, '-') << " | " 
              << std::string(10, '-') << " | " 
              << std::string(6, '-') << " | " 
              << std::string(6, '-') << " | " 
              << std::string(10, '-') << std::endl;
    
    // Find special indices
    double zeroThreshold = 0.0;
    bool hasZero = false;
    
    for (const auto& result : allResults) {
        std::string notes;
        
        // Check if default threshold
        if (std::abs(result.threshold) < 1e-9) {
            hasZero = true;
            zeroThreshold = result.cheeger;
            if (!notes.empty()) notes += ", ";
            notes += "DEFAULT";
        }
        
        // Check if best threshold
        if (std::abs(result.threshold - bestThreshold) < 1e-9) {
            if (!notes.empty()) notes += ", ";
            notes += "BEST";
        }
        
        // Add TOP 5 note
        for (size_t i = 0; i < std::min(size_t(5), allResults.size()); ++i) {
            if (std::abs(result.threshold - allResults[i].threshold) < 1e-9) {
                if (!notes.empty()) notes += ", ";
                notes += "TOP " + std::to_string(i + 1);
                break;
            }
        }
        
        std::cout << std::setw(12) << std::fixed << std::setprecision(6) << result.threshold << " | " 
                  << std::setw(10) << std::fixed << std::setprecision(6) << result.cheeger << " | " 
                  << std::setw(10) << result.cuts << " | " 
                  << std::setw(10) << result.volA << " | " 
                  << std::setw(10) << result.volB << " | " 
                  << std::setw(6) << result.sizeA << " | " 
                  << std::setw(6) << result.sizeB << " | " 
                  << notes << std::endl;
    }
    
    // Print improvement over default threshold
    if (hasZero && std::abs(bestThreshold) > 1e-9) {
        double improvement = (1.0 - bestCheeger / zeroThreshold) * 100.0;
        std::cout << "\nImprovement over default threshold: " << std::fixed << std::setprecision(2)
                  << improvement << "% reduction in Cheeger constant" << std::endl;
    }
    
    // Print summary
    std::cout << "\nOptimal threshold: " << std::fixed << std::setprecision(6) << bestThreshold << std::endl;
    std::cout << "Optimal Cheeger constant: " << std::fixed << std::setprecision(6) << bestCheeger << std::endl;
    std::cout << "Partition sizes: A=" << bestPartitionA.size() << ", B=" << bestPartitionB.size() << std::endl;
    
    // Return top 5 results vector for saving
    std::vector<Result> topResults(allResults.begin(), 
                                  allResults.begin() + std::min(size_t(5), allResults.size()));
    
    std::vector<double> topResultsData;
    for (const auto& result : topResults) {
        topResultsData.push_back(result.threshold);
        topResultsData.push_back(result.cheeger);
        topResultsData.push_back(static_cast<double>(result.cuts));
        topResultsData.push_back(static_cast<double>(result.volA));
        topResultsData.push_back(static_cast<double>(result.volB));
        topResultsData.push_back(static_cast<double>(result.sizeA));
        topResultsData.push_back(static_cast<double>(result.sizeB));
    }
    
    return {bestPartitionA, bestPartitionB, bestThreshold, topResultsData, bestCheeger, bestCuts, bestVolA, bestVolB};
}

// Main spectral partitioning function
std::tuple<std::vector<int>, std::vector<int>, double, std::vector<double>, double, std::vector<std::tuple<double, double, int, int, int, std::vector<int>, std::vector<int>>>>
spectralPartitioning(const VertexList& vertices, const EdgeList& edges) {
    int numVertices = vertices.size();
    
    // Build adjacency list (more memory efficient than matrix)
    AdjacencyList adjList = buildAdjacencyList(numVertices, edges);
    
    // Compute degrees
    DegreeVector degrees = computeDegrees(adjList);
    
    // Compute Fiedler vector
    std::vector<double> fiedlerVector = computeFiedlerVectorLanczos(adjList, degrees);
    
    // Find optimal threshold
    auto [partitionA, partitionB, threshold, topResultsData, cheeger, cuts, volA, volB] = 
        findOptimalThresholdBisection(fiedlerVector, adjList, degrees);
    
    // Extract top 5 results for saving
    std::vector<std::tuple<double, double, int, int, int, std::vector<int>, std::vector<int>>> topResults;
    
    // First pass to get partitions for each top result
    for (size_t i = 0; i < topResultsData.size() / 7; ++i) {
        double resultThreshold = topResultsData[i*7];
        double resultCheeger = topResultsData[i*7 + 1];
        int resultCuts = static_cast<int>(topResultsData[i*7 + 2]);
        int resultVolA = static_cast<int>(topResultsData[i*7 + 3]);
        int resultVolB = static_cast<int>(topResultsData[i*7 + 4]);
        
        // Re-create partitions for this threshold
        std::vector<int> resultPartA;
        std::vector<int> resultPartB;
        
        for (size_t j = 0; j < fiedlerVector.size(); ++j) {
            if (fiedlerVector[j] >= resultThreshold) {
                resultPartA.push_back(j + 1);  // 1-indexed
            } else {
                resultPartB.push_back(j + 1);  // 1-indexed
            }
        }
        
        topResults.emplace_back(resultThreshold, resultCheeger, resultCuts, 
                               resultVolA, resultVolB, resultPartA, resultPartB);
    }
    
    std::cout << "\nSpectral partitioning final results:" << std::endl;
    std::cout << "- Optimal threshold: " << std::fixed << std::setprecision(6) << threshold << std::endl;
    std::cout << "- Partition A: " << partitionA.size() << " vertices (volume: " << volA << ")" << std::endl;
    std::cout << "- Partition B: " << partitionB.size() << " vertices (volume: " << volB << ")" << std::endl;
    std::cout << "- Cut size: " << cuts << " edges" << std::endl;
    std::cout << "- Cheeger constant: " << std::fixed << std::setprecision(6) << cheeger << std::endl;
    
    return {partitionA, partitionB, cheeger, fiedlerVector, threshold, topResults};
}

// Main function
int main(int argc, char* argv[]) {
    // Default parameters
    bool generateGraph = false;
    std::string inputFile;
    std::string graphType = "random";
    int numVertices = 50;
    int dimensions = 2;
    double avgDegree = 6.0;
    double minCoord = 0.0;
    double maxCoord = 1.0;
    bool ensureConnected = true;
    int rows = 5;
    int cols = 5;
    bool connectDiagonals = false;
    double radius = 1.0;
    int connectNeighbors = 1;
    std::string outputFile = "geometric_graph.dimacs";
    bool visualize = false;  // Not implemented in C++ version
    bool spectralCut = false;
    std::string imageFile;   // Not implemented in C++ version
    
    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--generate") {
            generateGraph = true;
        }
        else if (arg == "--read" && i+1 < argc) {
            inputFile = argv[++i];
        }
        else if (arg == "--type" && i+1 < argc) {
            graphType = argv[++i];
        }
        else if (arg == "--vertices" && i+1 < argc) {
            numVertices = std::stoi(argv[++i]);
        }
        else if (arg == "--dimensions" && i+1 < argc) {
            dimensions = std::stoi(argv[++i]);
        }
        else if (arg == "--avg_degree" && i+1 < argc) {
            avgDegree = std::stod(argv[++i]);
        }
        else if (arg == "--min_coord" && i+1 < argc) {
            minCoord = std::stod(argv[++i]);
        }
        else if (arg == "--max_coord" && i+1 < argc) {
            maxCoord = std::stod(argv[++i]);
        }
        else if (arg == "--ensure_connected") {
            ensureConnected = true;
        }
        else if (arg == "--not_connected") {
            ensureConnected = false;
        }
        else if (arg == "--rows" && i+1 < argc) {
            rows = std::stoi(argv[++i]);
        }
        else if (arg == "--cols" && i+1 < argc) {
            cols = std::stoi(argv[++i]);
        }
        else if (arg == "--connect_diagonals") {
            connectDiagonals = true;
        }
        else if (arg == "--radius" && i+1 < argc) {
            radius = std::stod(argv[++i]);
        }
        else if (arg == "--connect_neighbors" && i+1 < argc) {
            connectNeighbors = std::stoi(argv[++i]);
        }
        else if (arg == "--output" && i+1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "--spectral_cut") {
            spectralCut = true;
        }
        else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --generate                Generate a new graph" << std::endl;
            std::cout << "  --read <file>             Read graph from DIMACS file" << std::endl;
            std::cout << "  --type <type>             Type of graph: random, grid, circle" << std::endl;
            std::cout << "  --vertices <n>            Number of vertices" << std::endl;
            std::cout << "  --dimensions <d>          Dimensions for random graph" << std::endl;
            std::cout << "  --avg_degree <d>          Target average degree" << std::endl;
            std::cout << "  --min_coord <val>         Minimum coordinate value" << std::endl;
            std::cout << "  --max_coord <val>         Maximum coordinate value" << std::endl;
            std::cout << "  --ensure_connected        Ensure the graph is connected" << std::endl;
            std::cout << "  --not_connected           Allow disconnected graph" << std::endl;
            std::cout << "  --rows <n>                Number of rows in grid graph" << std::endl;
            std::cout << "  --cols <n>                Number of columns in grid graph" << std::endl;
            std::cout << "  --connect_diagonals       Connect diagonal neighbors in grid graph" << std::endl;
            std::cout << "  --radius <r>              Radius of circle graph" << std::endl;
            std::cout << "  --connect_neighbors <k>   Number of neighbors to connect in circle graph" << std::endl;
            std::cout << "  --output <file>           Output filename" << std::endl;
            std::cout << "  --spectral_cut            Apply spectral partitioning" << std::endl;
            return 0;
        }
    }
    
    // Input validation
    if (!generateGraph && inputFile.empty()) {
        std::cerr << "Error: Must specify either --generate or --read <file>" << std::endl;
        return 1;
    }
    
    VertexList vertices;
    EdgeList edges;
    
    // Generate or read graph
    auto startTime = std::chrono::high_resolution_clock::now();
    
    if (generateGraph) {
        // Generate based on type
        if (graphType == "random") {
            auto [generatedVertices, generatedEdges] = generateRandomGeometricGraph(
            numVertices, avgDegree, dimensions, minCoord, maxCoord, ensureConnected);
            vertices = generatedVertices;
            edges = generatedEdges;
        }  
        else if (graphType == "grid") {
            // For grid, generate vertices too
            vertices.clear();
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    vertices.push_back({static_cast<double>(r), static_cast<double>(c)});
                }
            }
            edges = generateGridGraph(rows, cols, connectDiagonals);
        }
        else if (graphType == "circle") {
            // For circle, generate vertices too
            vertices.clear();
            for (int i = 0; i < numVertices; ++i) {
                double angle = 2.0 * M_PI * i / numVertices;
                double x = radius * std::cos(angle);
                double y = radius * std::sin(angle);
                vertices.push_back({x, y});
            }
            edges = generateCircleGraph(numVertices, radius, connectNeighbors);
            
            // Ensure connected if needed
            if (ensureConnected && connectNeighbors < 1) {
                edges = ensureConnectedGraph(vertices, edges);
            }
        }
        else {
            std::cerr << "Unknown graph type: " << graphType << std::endl;
            return 1;
        }
        
        // Save to DIMACS format
        saveDimacsFormat(vertices, edges, outputFile);
        
        std::cout << "Generated " << graphType << " graph with " 
                  << vertices.size() << " vertices and " 
                  << edges.size() << " edges" << std::endl;
    }
    else {
        // Read from file
        std::tie(vertices, edges) = readDimacsFormat(inputFile);
        std::cout << "Read graph with " << vertices.size() << " vertices and " 
                  << edges.size() << " edges from " << inputFile << std::endl;
        
        // Ensure connected if requested
        if (ensureConnected) {
            size_t originalEdgeCount = edges.size();
            edges = ensureConnectedGraph(vertices, edges);
            if (edges.size() > originalEdgeCount) {
                std::cout << "Added " << (edges.size() - originalEdgeCount) 
                          << " edges to ensure connectivity." << std::endl;
                saveDimacsFormat(vertices, edges, outputFile);
            }
        }
        
        graphType = "imported";
    }
    
    // Apply spectral partitioning if requested
    if (spectralCut) {
        auto [partitionA, partitionB, cheeger, fiedlerVector, threshold, topResults] = 
            spectralPartitioning(vertices, edges);
        
        // Save top 5 cuts
        std::string basePath;
        size_t lastSlash = outputFile.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            basePath = outputFile.substr(0, lastSlash + 1);
        }
        
        std::string baseFilename = outputFile;
        size_t lastDot = baseFilename.find_last_of(".");
        if (lastDot != std::string::npos) {
            baseFilename = baseFilename.substr(0, lastDot);
        }
        
        std::cout << "\nSaving top 5 cuts to separate files:" << std::endl;
        
        for (size_t i = 0; i < topResults.size(); ++i) {
            const auto& [resultThreshold, resultCheeger, resultCuts, resultVolA, resultVolB, resultPartA, resultPartB] = topResults[i];
            
            std::string cutFilename = basePath + baseFilename + "_cut" + std::to_string(i+1) + ".cut";
            saveCutToFile(resultPartA, resultPartB, resultThreshold, resultCheeger, 
                         resultCuts, resultVolA, resultVolB, cutFilename);
            
            std::cout << "Cut " << (i+1) << ": Cheeger = " << std::fixed << std::setprecision(6) 
                      << resultCheeger << ", Threshold = " << resultThreshold 
                      << ", saved to " << cutFilename << std::endl;
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "\nTotal execution time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    
    return 0;
}
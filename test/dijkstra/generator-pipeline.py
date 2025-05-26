import os
from random import randint
from heapq import heappush, heappop

CODE_DIR = os.path.dirname(os.path.realpath(__file__))
debug_flag = os.getenv('DEBUG_SO') is not None


def dijkstra(graph, source, num_nodes):
    """Dijkstra implementation to generate expected outputs"""
    distance = [float('inf')] * num_nodes
    distance[source] = 0
    pq = [(0, source)]

    while pq:
        d, u = heappop(pq)

        if d > distance[u]:
            continue

        for v, weight in graph[u]:
            if distance[u] + weight < distance[v]:
                distance[v] = distance[u] + weight
                heappush(pq, (distance[v], v))

    return distance


def write_testcase(testcase_id, num_nodes, edges):
    """Writes the test case and its expected output"""
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{num_nodes} {len(edges)}\n")
        for u, v, w in edges:
            file.write(f"{u} {v} {w}\n")

    if debug_flag:
        # Build the graph for Dijkstra
        graph = [[] for _ in range(num_nodes)]
        for u, v, w in edges:
            graph[u].append((v, w))
            graph[v].append((u, w))  # For undirected graph

        # Calculate distances from node 0
        distances = dijkstra(graph, 0, num_nodes)

        with open(f"{testcase_id}.out", "w") as file:
            file.write(" ".join(str(d if d != float('inf') else -1)
                       for d in distances))


# Generate small test cases
for n in range(1, 3):
    num_nodes = 10**n
    for i in range(2 if debug_flag else 1):
        # Generate a random tree to ensure the graph is connected
        edges = []
        for v in range(1, num_nodes):
            u = randint(0, v - 1)
            weight = randint(1, 1000)
            edges.append((u, v, weight))

        # Add some additional random edges
        extra_edges = num_nodes // 2
        for _ in range(extra_edges):
            u = randint(0, num_nodes - 1)
            v = randint(0, num_nodes - 1)
            if u != v:
                weight = randint(1, 1000)
                edges.append((u, v, weight))

        write_testcase(f"{CODE_DIR}/T{n}_{i}", num_nodes, edges)

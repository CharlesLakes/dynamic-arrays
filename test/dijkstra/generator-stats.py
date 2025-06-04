import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))

file_cleaned = set()
count_write = 0
write_acumulation = ""


def create_or_clear(filename):
    with open(f"{CODE_DIR}/{filename}.in", "w") as file:
        file.write("")


def write(filename, content):
    global count_write
    global write_acumulation

    count_write += 1
    count_write %= 100

    write_acumulation += content

    if count_write != 0:
        return

    if filename not in file_cleaned:
        create_or_clear(filename)
        file_cleaned.add(filename)

    with open(f"{CODE_DIR}/{filename}.in", "a") as file:
        file.write(write_acumulation)

    write_acumulation = ""


# Generate graphs with different densities
for n in range(1, 9):
    num_nodes = 10**n

    # Sparse graph (O(n) edges)
    edges_sparse = num_nodes * 2
    write(f"T{n}_sparse", f"{num_nodes} {edges_sparse}\n")

    edges_added = set()

    # Step 1: Connect all nodes in a linear chain (spanning tree)
    for i in range(num_nodes - 1):
        u = i
        v = i + 1
        weight = randint(1, 1000)
        edges_added.add((u, v))
        edges_added.add((v, u))
        write(f"T{n}_sparse", f"{u} {v} {weight}\n")

    # Step 2: Add additional random edges without repetition and no loops
    extra_edges = edges_sparse - (num_nodes - 1)
    while extra_edges > 0:
        u = randint(0, num_nodes - 1)
        v = randint(0, num_nodes - 1)
        if u == v:
            continue
        if (u, v) in edges_added or (v, u) in edges_added:
            continue
        edges_added.add((u, v))
        edges_added.add((v, u))
        weight = randint(1, 1000)
        write(f"T{n}_sparse", f"{u} {v} {weight}\n")
        extra_edges -= 1

    count_write = -1
    write(f"T{n}_sparse", "")

    # Dense graph (O(n^2) edges, max n*(n-1)/2)
    if n <= 4:  # Only for small graphs to avoid huge test cases
        max_edges = (num_nodes * (num_nodes - 1)) // 2
        edges_dense = min(num_nodes * num_nodes // 4, max_edges)
        write(f"T{n}_dense", f"{num_nodes} {edges_dense}\n")

        edges_added = set()

        # Step 1: Spanning tree (chain)
        for i in range(num_nodes - 1):
            u = i
            v = i + 1
            weight = randint(1, 1000)
            edges_added.add((u, v))
            edges_added.add((v, u))
            write(f"T{n}_dense", f"{u} {v} {weight}\n")

        # Step 2: Additional random edges
        extra_edges = edges_dense - (num_nodes - 1)
        while extra_edges > 0:
            u = randint(0, num_nodes - 1)
            v = randint(0, num_nodes - 1)
            if u == v:
                continue
            if (u, v) in edges_added or (v, u) in edges_added:
                continue
            edges_added.add((u, v))
            edges_added.add((v, u))
            weight = randint(1, 1000)
            write(f"T{n}_dense", f"{u} {v} {weight}\n")
            extra_edges -= 1

        count_write = -1
        write(f"T{n}_dense", "")
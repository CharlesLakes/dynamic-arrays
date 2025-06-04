import math
import sys


def distance(p1, p2):
    """Returns the integer Euclidean distance between two points."""
    dx = p1[0] - p2[0]
    dy = p1[1] - p2[1]
    return int(math.hypot(dx, dy))


def parse_input():
    """Parses DIMACS input from stdin and returns vertices, coordinates, and edges."""
    n_vertices = n_edges = 0
    vertex_coords = {}
    edges = []

    # Read until the 'p edge n m' line is found
    for line in sys.stdin:
        tokens = line.strip().split()
        if not tokens:
            continue
        if tokens[0] == 'p' and tokens[1] == 'edge':
            n_vertices = int(tokens[2])
            n_edges = int(tokens[3])
            break

    # Read vertex coordinates
    count = 0
    while count < n_vertices:
        line = sys.stdin.readline().strip().split()
        if len(line) >= 5 and line[0] == 'c' and line[1] == 'v':
            idx = int(line[2])
            x = int(float(line[3]) * 1000)
            y = int(float(line[4]) * 1000)
            vertex_coords[idx] = (x, y)
            count += 1

    # Read edges
    count = 0
    while count < n_edges:
        line = sys.stdin.readline().strip().split()
        if len(line) >= 3 and line[0] == 'e':
            u, v = int(line[1]), int(line[2])
            edges.append((u, v))
            count += 1

    return n_vertices, n_edges, vertex_coords, edges


def main():
    n, m, coords, edges = parse_input()
    print(n, m)
    for u, v in edges:
        w = distance(coords[u], coords[v])
        print(u - 1, v - 1, w)


if __name__ == "__main__":
    main()

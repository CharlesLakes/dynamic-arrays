import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))
debug_flag = os.getenv('DEBUG_SO') is not None


def write_testcase(testcase_id, array, queries):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array)}\n")
        file.write(f"{' '.join(map(str,array))}\n")
        file.write(f"{len(queries)}\n")
        for query in queries:
            op, value = query
            if op:
                file.write(f"{op} {value}\n")
            else:
                file.write(f"{op}\n")

    for query in queries:
        op, value = query
        if op:
            array.append(value)
        else:
            array.pop()

    with open(f"{testcase_id}.out", "w") as file:
        file.write(f"{' '.join(map(str,array))}")


for n in range(1, 3 if debug_flag else 10):
    for i in range(2 if debug_flag else 1):
        array = [randint(1, 1000000000) for _ in range(10**n)]
        queries = [(randint(0, 1), randint(1, 1000000000))
                   for _ in range(10**n)]

        write_testcase(f"{CODE_DIR}/T{n}_{i}", array, queries)

for n in range(1, 3 if debug_flag else 10):
    for i in range(2 if debug_flag else 1):
        array = [randint(1, 1000000000) for _ in range(10**n)]

        push_queries = [(1, randint(1, 1000000000)) for _ in range(10**n // 2)]
        random_queries = [
            (randint(
                0, 1), randint(
                1, 1000000000)) for _ in range(
                10**n // 2)]
        combined_queries_1 = push_queries + random_queries
        write_testcase(
            f"{CODE_DIR}/T{n}_{i}_push_then_random",
            array,
            combined_queries_1)

        random_queries = [
            (randint(
                0, 1), randint(
                1, 1000000000)) for _ in range(
                10**n // 2)]
        pop_queries = [(0, 0) for _ in range(10**n // 2)]
        combined_queries_2 = random_queries + pop_queries
        write_testcase(
            f"{CODE_DIR}/T{n}_{i}_random_then_pop",
            array,
            combined_queries_2)

        push_queries = [(1, randint(1, 1000000000)) for _ in range(10**n // 3)]
        random_queries = [
            (randint(
                0, 1), randint(
                1, 1000000000)) for _ in range(
                10**n // 3)]
        pop_queries = [(0, 0) for _ in range(10**n // 3)]
        combined_queries_3 = push_queries + random_queries + pop_queries
        write_testcase(
            f"{CODE_DIR}/T{n}_{i}_push_random_then_pop",
            array,
            combined_queries_3)

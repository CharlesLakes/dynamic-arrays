import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))
debug_flag = os.getenv('DEBUG_SO') is not None


def write_testcase(testcase_id, array, queries):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array)}\n")
        file.write(f"{' '.join(map(str, array))}\n")
        file.write(f"{len(queries)}\n")
        file.write(f"{' '.join(map(str, queries))}")

    if debug_flag:
        set_array = set(array)
        answer = [int(query in set_array) for query in queries]

        with open(f"{testcase_id}.out", "w") as file:
            file.write(f"{' '.join(map(str, answer))}")


for n in range(1, 2):
    for i in range(2):
        array = [randint(1, 10**n) for _ in range(10**n)]
        queries = [randint(1, 10**n) for _ in range(10**n)]
        write_testcase(f"{CODE_DIR}/T{n}_{i}", array, queries)

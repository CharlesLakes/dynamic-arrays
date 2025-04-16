import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))
debug_flag = os.getenv('DEBUG_SO') is not None


def write_testcase(testcase_id, array):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array)}\n")
        file.write(f"{' '.join(map(str, array))}")

    if debug_flag:
        array.sort()

        with open(f"{testcase_id}.out", "w") as file:
            file.write(f"{' '.join(map(str, array))}")


for n in range(1, 3):
    for i in range(2 if debug_flag else 1):
        array = [randint(1, 1000000000) for _ in range(10**n)]
        write_testcase(f"{CODE_DIR}/T{n}_{i}", array)

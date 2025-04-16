import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))


def write_testcase(testcase_id, array, queries):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array)}\n")
        file.write(f"{' '.join(map(str, array))}\n")
        file.write(f"{len(queries)}\n")
        for query in queries:
            op, value = query
            if op:
                file.write(f"{op} {value}\n")
            else:
                file.write(f"{op}\n")

    array = list(array)
    for query in queries:
        op, value = query
        if op:
            array.append(value)
        else:
            array.pop()

    array.reverse()

    with open(f"{testcase_id}.out", "w") as file:
        file.write(f"{' '.join(map(str, array))}")


for n in range(1, 3):
    for i in range(2):
        array = [randint(1, 1000000000) for _ in range(10**n)]
        queries = [(randint(0, 1), randint(1, 1000000000))
                   for _ in range(10**n)]

        write_testcase(f"{CODE_DIR}/T{n}_{i}", array, queries)

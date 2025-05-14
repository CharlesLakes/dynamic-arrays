import os
from random import randint
from MaxHeap import MaxHeap


CODE_DIR = os.path.dirname(os.path.realpath(__file__))


def write_testcase(testcase_id, array, queries):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array) + len(queries)}\n")

        for value in array:
            file.write(f"1 {value}\n")

        for query in queries:
            if query[0]:
                file.write(f"1 {query[1]}\n")
            else:
                file.write(f"0\n")

    with open(f"{testcase_id}.out", "w") as file:
        pq = MaxHeap()
        for value in array:
            pq.push(value)
            file.write(f"{pq.top()}\n")
        for query in queries:
            if query[0]:
                pq.push(query[1])
                file.write(f"{pq.top()}\n")
            else:
                pq.pop()
                file.write(f"{pq.top()}\n")


for n in range(1, 5):
    for i in range(2):
        array = [randint(1, 1000000000) for _ in range(10**n)]
        queries = [(randint(0, 1), randint(1, 1000000000))
                   for _ in range(10**n)]

        write_testcase(f"{CODE_DIR}/T{n}_{i}", array, queries)

from random import randint


def write_testcase(testcase_id, array):
    with open(f"{testcase_id}.in", "w") as file:
        file.write(f"{len(array)}\n")
        file.write(f"{' '.join(map(str,array))}")

    array.sort()

    with open(f"{testcase_id}.out", "w") as file:
        file.write(f"{' '.join(map(str,array))}")


# T1
# 10 testcase de arreglos con largo 10, con un interavlo de [1,100]
for i in range(20):
    array = [randint(1, 100) for _ in range(10)]

    write_testcase(f"T1_{i}", array)


# T2
# 10 testcase de arreglos con largo 1000, con un interavlo de [1,10000]
for i in range(20):
    array = [randint(1, 10000) for _ in range(1000)]

    write_testcase(f"T2_{i}", array)

# T3
# 10 testcase de arreglos con largo 100000, con un interavlo de [1,1000000000]
for i in range(20):
    array = [randint(1, 1000000000) for _ in range(100000)]

    write_testcase(f"T3_{i}", array)

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
    count_write %= 100000

    write_acumulation += content

    if count_write != 0:
        return

    if filename not in file_cleaned:
        create_or_clear(filename)
        file_cleaned.add(filename)

    with open(f"{CODE_DIR}/{filename}.in", "a") as file:
        file.write(write_acumulation)

    write_acumulation = ""


for n in range(1, 5):

    size = 10**n

    write(
        f"T{n}",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    write(
        f"T{n}",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    count_write = -1
    write(f"T{n}", "")

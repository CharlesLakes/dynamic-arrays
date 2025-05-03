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


# --- random ---

for n in range(1, 10):
    size = 10**n

    write(
        f"T{n}_random",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_random",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    write(
        f"T{n}_random",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_random",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

    count_write = -1
    write(f"T{n}_random", "")

# --- push then random ---

for n in range(1, 10):
    size = 10**n

    write(
        f"T{n}_push_then_random",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_push_then_random",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    queries = (size // 2) * 2

    write(
        f"T{n}_push_then_random",
        f"{queries}\n"
    )

    # -- push --

    for i in range(size // 2):
        write(
            f"T{n}_push_then_random",
            f"{1} {randint(1, 1000000000)}\n"
        )

    # -- random --

    for i in range(size // 2):
        write(
            f"T{n}_push_then_random",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

    count_write = -1
    write(f"T{n}_push_then_random", "")

# -- random then pop --

for n in range(1, 10):
    size = 10**n

    write(
        f"T{n}_random_then_pop",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_random_then_pop",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    queries = (size // 2) * 2

    write(
        f"T{n}_random_then_pop",
        f"{queries}\n"
    )

    # -- random --

    for i in range(size // 2):
        write(
            f"T{n}_random_then_pop",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

    # -- pop --

    for i in range(size // 2):
        write(
            f"T{n}_random_then_pop",
            f"{0} {0}\n"
        )

    count_write = -1
    write(f"T{n}_random_then_pop", "")

# --- push random then pop ---

for n in range(1, 10):
    size = 10**n

    write(
        f"T{n}_push_random_then_pop",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_push_random_then_pop",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    queries = (size // 3) * 3

    write(
        f"T{n}_push_random_then_pop",
        f"{queries}\n"
    )

    # -- push --

    for i in range(size // 3):
        write(
            f"T{n}_push_random_then_pop",
            f"{1} {randint(1, 1000000000)}\n"
        )

    # -- random --

    for i in range(size // 3):
        write(
            f"T{n}_push_random_then_pop",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

    # -- pop --

    for i in range(size // 3):
        write(
            f"T{n}_push_random_then_pop",
            f"{0} {0}\n"
        )

    count_write = -1
    write(f"T{n}_push_random_then_pop", "")

# n/2 push + n/2 pop

for n in range(1, 10):
    size = 10**n

    write(
        f"T{n}_push_then_pop",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_push_then_pop",
            str(randint(1, 1000000000)) + (" " if size > i + 1 else "\n")
        )

    queries = (size // 2) * 2

    # -- push --

    for i in range(size // 3):
        write(
            f"T{n}_push_then_pop",
            f"{1} {randint(1, 1000000000)}\n"
        )

    # -- pop --

    for i in range(size // 3):
        write(
            f"T{n}_push_then_pop",
            f"{0} {0}\n"
        )

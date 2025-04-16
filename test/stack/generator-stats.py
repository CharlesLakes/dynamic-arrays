import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))

def create_or_clear(filename):
    with open(f"{CODE_DIR}/{filename}.in","w") as file:
        file.write("") 

def write(filename,content):
    with open(f"{CODE_DIR}/{filename}.in","a") as file:
        file.write(content)



# --- push then random ---

for n in range(1,10):
    size = 10**n

    write(
        f"T{n}_push_then_random",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_push_then_random",
            str(randint(1,1000000000)) + (" " if size > i + 1 else "\n")
        )
    
    queries = (size//2)*2

    write(
        f"T{n}_push_then_random",
        f"{queries}\n"
    )

    # -- push --

    for i in range(size//2):
        write(
            f"T{n}_push_then_random",
            f"{1} {randint(1, 1000000000)}\n"
        )

    # -- random --

    for i in range(size//2):
        write(
            f"T{n}_push_then_random",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

# -- random then pop --

for n in range(1,10):
    size = 10**n

    write(
        f"T{n}_random_then_pop",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_random_then_pop",
            str(randint(1,1000000000)) + (" " if size > i + 1 else "\n")
        )
    
    queries = (size//2)*2

    write(
        f"T{n}_random_then_pop",
        f"{queries}\n"
    )

    # -- random --

    for i in range(size//2):
        write(
            f"T{n}_random_then_pop",
            f"{randint(0, 1)} {randint(1, 1000000000)}\n"
        )

    # -- pop --

    for i in range(size//2):
        write(
            f"T{n}_random_then_pop",
            f"{0} {0}\n"
        )

# --- push random then pop ---

for n in range(1,10):
    size = 10**n

    write(
        f"T{n}_push_random_then_pop",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}_push_random_then_pop",
            str(randint(1,1000000000)) + (" " if size > i + 1 else "\n")
        )

    queries = (size//3)*3

    write(
        f"T{n}_push_random_then_pop",
        f"{queries}\n"
    )

    # -- push --

    for i in range(size//3):
        write(
            f"T{n}_push_random_then_pop",
            f"{1} {randint(1, 1000000000)}\n"
        )

    # -- random -- 

    for i in range(size//3):
        write(
            f"T{n}_push_random_then_pop",
            f"{randint(0,1)} {randint(1, 1000000000)}\n"
        )

    # -- pop --

    for i in range(size//3):
        write(
            f"T{n}_push_random_then_pop",
            f"{0} {0}\n"
        )
import os
from random import randint

CODE_DIR = os.path.dirname(os.path.realpath(__file__))

file_cleaned = set()

def create_or_clear(filename):
    with open(f"{CODE_DIR}/{filename}.in","w") as file:
        file.write("") 

def write(filename,content):
    if filename not in file_cleaned:
        create_or_clear(filename)
        file_cleaned.add(filename)

    with open(f"{CODE_DIR}/{filename}.in","a") as file:
        file.write(content)

for n in range(1, 10):

    size = 10**n

    write(
        f"T{n}",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}",
            str(randint(1,1000000000)) + (" " if size > i + 1 else "\n")
        )

    write(
        f"T{n}",
        f"{size}\n"
    )

    for i in range(size):
        write(
            f"T{n}",
            str(randint(1,1000000000)) + (" " if size > i + 1 else "\n")
        )
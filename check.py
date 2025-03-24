import sys
import os
from time import time, process_time


def compile(code, compiler):
    return os.system(f"{compiler} {code} -o code.out")


def get_memory_usage():
    """ Obtiene la memoria máxima usada en heap y stack desde Massif. """
    with open("massif.out", "r") as f:
        heap_usage = 0
        stack_usage = 0
        for line in f:
            if "mem_heap_B=" in line:
                heap_usage = max(heap_usage, int(line.split('=')[1]))
            elif "mem_stacks_B=" in line:
                stack_usage = max(stack_usage, int(line.split('=')[1]))
    return heap_usage, stack_usage

def main(code_path, dir_folder, compiler="g++"):
    if compile(code_path, compiler):
        print("Error al compilar.")
        return

    files = os.listdir(dir_folder)
    print(f"--- START: {dir_folder} ---")
    for file in files:
        if file.endswith(".in"):
            testcase_name = file[:-3]

            start_wall = time()
            start_cpu = process_time()

            error = os.system(
                f'valgrind --tool=massif --massif-out-file=massif.out ./code.out < "{dir_folder}/{testcase_name}.in" > ans.out'
            )

            end_wall = time()
            end_cpu = process_time()

            wall_time = round(end_wall - start_wall, 6)
            cpu_time = round(end_cpu - start_cpu, 6)

            if error:
                print(f"Error o fuga de memoria en {testcase_name}.")
                return

            check = os.system(
                f'diff -q --ignore-trailing-space "{dir_folder}/{testcase_name}.out" ans.out')
            if check:
                print(f"No match ({testcase_name}).")
                return

            heap_used, stack_used = get_memory_usage()

            print(
                f"AC - {testcase_name} | Wall Time: {wall_time}s | CPU Time: {cpu_time}s | Heap: {heap_used}B | Stack: {stack_used}B"
            )

    print(f"--- END: {dir_folder} ---")

if __name__ == "__main__":
    main(*sys.argv[1:])

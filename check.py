import sys
import os
from time import time, process_time


def compile(code, compiler):
    return os.system(f"{compiler} {code} -o code.out")


def get_memory_usage():
    """ Gets the maximum heap and stack memory usage from Massif. """
    heap_usage, stack_usage = 0, 0
    try:
        with open("massif.out", "r") as f:
            for line in f:
                if line.startswith("mem_heap_B="):
                    heap_usage = max(heap_usage, int(line.split('=')[1]))
                elif line.startswith("mem_stacks_B="):
                    stack_usage = max(stack_usage, int(line.split('=')[1]))
    except FileNotFoundError:
        print("Error: massif.out file not found.")
        exit(1)  # Stop execution if Massif output is missing.
    return heap_usage, stack_usage


def main(code_path, dir_folder, compiler="g++"):
    if compile(code_path, compiler):
        print("Compilation error.")
        exit(1)

    files = os.listdir(dir_folder)

    for file in files:
        if file.endswith(".in"):
            testcase_name = file[:-3]

            start_wall = time()
            start_cpu = process_time()

            error = os.system(
                f'valgrind --leak-check=full --error-exitcode=1 --quiet '
                f'--tool=massif --massif-out-file=massif.out ./code.out < "{dir_folder}/{testcase_name}.in" > ans.out')

            end_wall = time()
            end_cpu = process_time()

            wall_time = round(end_wall - start_wall, 6)
            cpu_time = round(end_cpu - start_cpu, 6)

            if error:
                print(f"Error: Memory leak detected.")
                exit(1)

            check = os.system(
                f'diff -q --ignore-trailing-space "{dir_folder}/{testcase_name}.out" ans.out')
            if check:
                print(f"No match ({testcase_name}).")
                exit(1)

            heap_used, stack_used = get_memory_usage()

            print(
                f"AC - {testcase_name} | Wall Time: {wall_time}s | CPU Time: {cpu_time}s | "
                f"Heap: {heap_used}B | Stack: {stack_used}B")


if __name__ == "__main__":
    main(*sys.argv[1:])

import sys
import os
from time import time, process_time

def compile(code, compiler):
    return os.system(f"{compiler} {code} -o code.out")

def main(code_path, dir_folder, compiler = "g++"):
    if compile(code_path, compiler):
        print("Error al compilar.")
        exit(1)

    files = os.listdir(dir_folder)

    for file in files:
        if file.endswith(".in"):
            testcase_name = file[:-3]

            start_wall = time()
            start_cpu = process_time()

            error = os.system(
                f'valgrind --leak-check=full --error-exitcode=1 --quiet ./code.out < "{dir_folder}/{testcase_name}.in" > ans.out'
            )

            end_wall = time()
            end_cpu = process_time()

            wall_time = end_wall - start_wall
            cpu_time = end_cpu - start_cpu

            if error:
                print(f"Error or memory leak.")
                exit(1)

            check = os.system(f'diff -q --ignore-trailing-space "{dir_folder}/{testcase_name}.out" ans.out')
            if check:
                print(f"No match ({testcase_name}).")
                exit(1)
            
            print(f"AC - {testcase_name} - Wall time: {wall_time} - CPU time: {cpu_time}")




if __name__ == "__main__":
    main(*sys.argv[1:])
    
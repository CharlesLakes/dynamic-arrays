import sys
import os

def compile(code):
    return os.system(f"g++ {code} -o code.out")

def main(code_path, dir_folder):
    if compile(code_path):
        print("Error al compilar.")
        exit(1)

    files = os.listdir(dir_folder)

    for file in files:
        if file.endswith(".in"):
            testcase_name = file[:-3]
            os.system(f'./code.out < "{dir_folder}/{testcase_name}.in" > ans.out')
            check = os.system(f'diff -q --ignore-trailing-space "{dir_folder}/{testcase_name}.out" ans.out')
            if check:
                print(f"No match ({testcase_name}).")
                exit(1)
            
            print(f"AC - {testcase_name}")




if __name__ == "__main__":
    main(*sys.argv[1:])
    
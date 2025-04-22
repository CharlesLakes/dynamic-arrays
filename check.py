import sys
import os
import subprocess
import resource
import psutil
from time import perf_counter, sleep


def compile_code(code, compiler, debug_flag=False):
    compile_args = [compiler, code, "-o", "code.out"]
    if debug_flag:
        compile_args.append("-DDEBUG_SO")  # Add -DDEBUG_SO if debug is enabled

    result = subprocess.run(
        compile_args,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    return result.returncode == 0


def get_memory_usage():
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
        sys.exit(1)
    return heap_usage, stack_usage


def measure_cpu_time_before():
    return resource.getrusage(resource.RUSAGE_CHILDREN)


def measure_cpu_time_after(before):
    after = resource.getrusage(resource.RUSAGE_CHILDREN)
    user_time = after.ru_utime - before.ru_utime
    system_time = after.ru_stime - before.ru_stime
    return user_time, system_time


def get_memory_usage_all(binary_path, input_path):
    process = subprocess.Popen(
        [binary_path],
        stdin=open(input_path, 'r'),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)

    pid = process.pid
    p = psutil.Process(pid)

    max_memory_usage = 0

    try:
        while process.poll() is None:
            mem_info = p.memory_info()
            max_memory_usage = max(max_memory_usage, mem_info.rss)
            sleep(0.001)

        mem_info = p.memory_info()
        max_memory_usage = max(max_memory_usage, mem_info.rss)

    except psutil.NoSuchProcess:
        print(f"The process with PID {pid} has already finished.")

    return max_memory_usage


def debug_mode(testcase_name, input_path, output_path):
    # Run valgrind --leak-check
    try:
        subprocess.run(
            ['valgrind', '--leak-check=full', '--error-exitcode=1', '--quiet', './code.out'],
            stdin=open(input_path, 'r'),
            stdout=open('ans.out', 'w'),
            stderr=subprocess.DEVNULL,
            check=True
        )
    except subprocess.CalledProcessError:
        print(f"Error: Memory leak detected in test case {testcase_name}.")
        sys.exit(1)

    # Check output correctness only if DEBUG_SO is defined
    result = subprocess.run(
        ['diff', '-q', '--ignore-trailing-space', output_path, 'ans.out'],
        stdout=subprocess.DEVNULL
    )
    if result.returncode != 0:
        print(f"No match ({testcase_name}).")
        sys.exit(1)

    # Run massif for memory usage
    subprocess.run(['valgrind',
                    '--tool=massif',
                    '--stacks=yes',
                    '--massif-out-file=massif.out',
                    './code.out'],
                   stdin=open(input_path,
                              'r'),
                   stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)
    heap_used, stack_used = get_memory_usage()

    # Run again for performance measurement
    wall_start = perf_counter()
    cpu_before = measure_cpu_time_before()

    subprocess.run(
        ['./code.out'],
        stdin=open(input_path, 'r'),
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

    cpu_user, cpu_sys = measure_cpu_time_after(cpu_before)
    wall_end = perf_counter()
    wall_time = wall_end - wall_start

    print(
        f"AC - {testcase_name} | Wall: {wall_time:.6f}s | CPU User: {cpu_user:.6f}s | "
        f"CPU Sys: {cpu_sys:.6f}s | Heap: {heap_used}B | Stack: {stack_used}B")


def normal_mode(testcase_name, input_path, output_path):
    # Run again for performance measurement
    wall_start = perf_counter()
    cpu_before = measure_cpu_time_before()

    subprocess.run(
        ['./code.out'],
        stdin=open(input_path, 'r'),
        stdout=open('ans.out', 'w'),
        stderr=subprocess.DEVNULL
    )

    cpu_user, cpu_sys = measure_cpu_time_after(cpu_before)
    wall_end = perf_counter()
    wall_time = wall_end - wall_start

    max_memory_usage = get_memory_usage_all('./code.out', input_path)

    print(
        f"AC - {testcase_name} | Wall: {wall_time:.6f}s | CPU User: {cpu_user:.6f}s | "
        f"CPU Sys: {cpu_sys:.6f}s | "
        f"Max Memory: {max_memory_usage}B"
        # f"Heap: {heap_used}B | Stack: {stack_used}B"
    )


def main(code_path, dir_folder, compiler="g++"):
    # Check if DEBUG_SO is defined in the environment
    debug_flag = os.getenv('DEBUG_SO') is not None

    if not compile_code(code_path, compiler, debug_flag):
        print("Compilation error.")
        sys.exit(1)

    for file in os.listdir(dir_folder):
        if not file.endswith(".in"):
            continue

        testcase_name = file[:-3]
        input_path = os.path.join(dir_folder, f"{testcase_name}.in")
        output_path = os.path.join(dir_folder, f"{testcase_name}.out")
        if debug_flag:
            debug_mode(testcase_name, input_path, output_path)
        else:
            normal_mode(testcase_name, input_path, output_path)


if __name__ == "__main__":
    main(*sys.argv[1:])

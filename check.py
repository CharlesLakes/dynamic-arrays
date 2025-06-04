import sys
import os
import subprocess
import resource
from time import perf_counter


def compile_code(code, compiler, debug_flag=False):
    compile_args = [compiler, code, 
                    "-O3", 
                    "-march=native", 
                    "-o", 
                    "code.out"]
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


def measure_performance(binary_path, input_path):
    result = subprocess.run(
        ['/usr/bin/time', '-v'] + [binary_path],
        stdin=open(input_path, 'r'),
        stderr=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        text=True
    )

    rss_kb = None
    user_time = None
    system_time = None

    for line in result.stderr.splitlines():
        if "Maximum resident set size" in line:
            rss_kb = int(line.split(":")[1].strip())
        elif "User time (seconds)" in line:
            user_time = float(line.split(":")[1].strip())
        elif "System time (seconds)" in line:
            system_time = float(line.split(":")[1].strip())

    if None in (rss_kb, user_time, system_time):
        raise ValueError("Failed to retrieve all metrics.")

    cpu_time_total = user_time + system_time
    rss_bytes = rss_kb * 1024

    return {
        "rss_bytes": rss_bytes,
        "user_time": user_time,
        "system_time": system_time,
        "cpu_time_total": cpu_time_total,
        "code": result.returncode
    }


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


def normal_mode(testcase_name, input_path):
    result = measure_performance("./code.out", input_path)

    wall_time = result["cpu_time_total"]
    cpu_user = result["user_time"]
    cpu_sys = result["system_time"]
    max_memory_usage = result["rss_bytes"]
    code = result["code"]

    if code != 0:
        print("Return code:", code)

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
            normal_mode(testcase_name, input_path)


if __name__ == "__main__":
    main(*sys.argv[1:])

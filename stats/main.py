import re
import sys

def extract_metrics(line):
    pattern = re.compile(
        r"AC - T(\d+)_\d+_?(.*) \| Wall: ([\d.]+)s \| CPU User: ([\d.]+)s \| CPU Sys: ([\d.]+)s \| Heap: (\d+)B \| Stack: (\d+)B"
    )
    match = pattern.search(line)
    if match:
        n_exp = int(match.group(1))
        type_testcase = match.group(2)
        wall_time = float(match.group(3))
        cpu_user = float(match.group(4))
        cpu_sys = float(match.group(5))
        heap_usage = int(match.group(6))
        stack_usage = int(match.group(7))
        return {
            "n": 10 ** n_exp,
            "type_testcase": type_testcase,
            "wall_time": wall_time,
            "cpu_user": cpu_user,
            "cpu_sys": cpu_sys,
            "heap": heap_usage,
            "stack": stack_usage
        }
    else:
        return None

import matplotlib.pyplot as plt

# Function to plot metrics for each execution
def plot_metrics(stats_by_execution):
    for execution_name, metrics_list in stats_by_execution.items():
        ns = [entry["n"] for entry in metrics_list]
        wall_times = [entry["wall_time"] for entry in metrics_list]
        cpu_users = [entry["cpu_user"] for entry in metrics_list]
        heaps = [entry["heap"] for entry in metrics_list]
        stacks = [entry["stack"] for entry in metrics_list]

        """
        # Plot wall time
        plt.figure(figsize=(14, 8))
        plt.plot(ns, wall_times, marker='o')
        plt.title(f"{execution_name.strip()} - Wall Time")
        plt.xlabel("n")
        plt.ylabel("Wall Time (s)")
        plt.ticklabel_format(style='plain', axis='x')
        plt.grid(True)
        plt.savefig(f"{execution_name.strip()}_wall_time.png")
        """


        # Plot CPU user time
        plt.figure(figsize=(14, 8))
        plt.plot(ns, cpu_users, marker='o', color='orange')
        plt.title(f"{execution_name.strip()} - CPU User Time")
        plt.xlabel("n")
        plt.ylabel("CPU User Time (s)")
        plt.ticklabel_format(style='plain', axis='x')
        plt.xlim(0, 10**7)
        plt.grid(True)
        plt.savefig(f"{execution_name.strip()}_cpu_user.png")


        # Plot Heap usage
        plt.figure(figsize=(14, 8))
        plt.plot(ns, heaps, marker='o', color='green')
        plt.title(f"{execution_name.strip()} - Heap Usage")
        plt.xlabel("n")
        plt.ylabel("Heap (Bytes)")
        plt.ticklabel_format(style='plain', axis='x')
        plt.xlim(0, 10**7)
        plt.grid(True)
        plt.savefig(f"{execution_name.strip()}_heap.png")

        """
        # Plot Stack usage
        plt.figure(figsize=(14, 8))
        plt.plot(ns, stacks, marker='o', color='red')
        plt.title(f"{execution_name.strip()} - Stack Usage")
        plt.xlabel("n")
        plt.ylabel("Stack (Bytes)")
        plt.ticklabel_format(style='plain', axis='x')
        plt.xlim(0, 10**7)
        plt.grid(True)
        plt.savefig(f"{execution_name.strip()}_stack.png")
        """

def main(stage_path):
    stats_by_execution = {}
    
    with open(stage_path) as file:
        current_execution = ""
        for line in file:
            line = line.strip()
            if line == "All tests completed.":
                continue

            metrics = extract_metrics(line)
            if metrics:
                type_execution = metrics["type_testcase"]
                
                key_group = f"{current_execution} - {type_execution}"

                if key_group not in stats_by_execution:
                    stats_by_execution[key_group] = []

                stats_by_execution[key_group].append(
                    metrics
                )
            else:
                line = line \
                    .replace("Running ","") \
                    .replace("...","")
                
                current_execution = line

    plot_metrics(stats_by_execution)

if __name__ == "__main__":
    main(*sys.argv[1:])
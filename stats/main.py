import matplotlib.pyplot as plt
import re
import sys
import math


def extract_metrics(line):
    pattern = re.compile(
        r"AC - T(\d+)_?(.*) \| Wall: ([\d.]+)s \| CPU User: ([\d.]+)s \| CPU Sys: ([\d.]+)s \| Max Memory: (\d+)B"
        # r"AC - T(\d+)_?(.*) \| Wall: ([\d.]+)s \| CPU User: ([\d.]+)s \| CPU Sys: ([\d.]+)s \| Heap: (\d+)B \| Stack: (\d+)B"
    )
    match = pattern.search(line)
    if match:
        n_exp = int(match.group(1))
        type_testcase = match.group(2)
        wall_time = float(match.group(3))
        cpu_user = float(match.group(4))
        cpu_sys = float(match.group(5))
        memory_usage = int(match.group(6))
        # heap_usage = int(match.group(6))
        # stack_usage = int(match.group(7))
        return {
            "n": 10 ** n_exp,
            "type_testcase": type_testcase,
            "wall_time": wall_time,
            "cpu_user": cpu_user,
            "cpu_sys": cpu_sys,
            "memory": memory_usage
            # "heap": heap_usage,
            # "stack": stack_usage
        }
    else:
        return None


# Function to plot metrics for each execution

def plot_metrics(stats_by_category):
    # Plot CPU User Time
    for category in stats_by_category:
        for type_execution in stats_by_category[category]:
            executions = list(
                stats_by_category[category][type_execution].keys())

            _, ax = plt.subplots(figsize=(10, 6))

            for current_execution in executions:
                executions_data = stats_by_category[category][type_execution][current_execution]
                aux_ns = [(execution_data["n"], i)
                          for i, execution_data in enumerate(executions_data)]
                aux_cpu_users = [execution_data["cpu_user"]
                                 for execution_data in executions_data]
                aux_ns.sort()

                ns = [execution_data for execution_data, _ in aux_ns]
                cpu_users = [aux_cpu_users[i] for _, i in aux_ns]

                ax.plot(ns, cpu_users, marker='o', label=current_execution)

            ax.set_title(f"{category} - {type_execution} - CPU User Time")
            ax.set_xlabel("n")
            ax.set_ylabel("CPU User Time (s)")
            ax.set_xscale('log')
            ax.ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
            ax.grid(True, which='both')
            ax.legend()
            plt.tight_layout()
            plt.savefig(f"{category}_{type_execution}_cpu_user.png")
            plt.close()

    # Plot Memory Usage
    for category in stats_by_category:
        for type_execution in stats_by_category[category]:
            executions = list(
                stats_by_category[category][type_execution].keys())

            _, ax = plt.subplots(figsize=(10, 6))

            for current_execution in executions:
                executions_data = stats_by_category[category][type_execution][current_execution]
                aux_ns = [(execution_data["n"], i)
                          for i, execution_data in enumerate(executions_data)]
                aux_memorys = [execution_data["memory"]
                               for execution_data in executions_data]
                aux_ns.sort()

                ns = [execution_data for execution_data, _ in aux_ns]
                memorys = [aux_memorys[i] for _, i in aux_ns]

                ax.plot(ns, memorys, marker='o', label=current_execution)

            ax.set_title(f"{category} - {type_execution} - Memory Usage")
            ax.set_xlabel("n")
            ax.set_ylabel("Memory (Bytes)")
            ax.set_xscale('log')
            ax.ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
            ax.grid(True, which='both')
            ax.legend()
            plt.tight_layout()
            plt.savefig(f"{category}_{type_execution}_memory.png")
            plt.close()


def get_category(line, categories):
    for category in categories:
        if category in line:
            return category
    return None


def main(stage_path):
    stats_by_category = {}

    testcases_category = [
        "stack",
        "binary search",
        "linear search",
        "sort"
    ]

    with open(stage_path) as file:
        current_execution = ""
        for line in file:
            line = line.strip()
            if line == "All tests completed.":
                continue

            metrics = extract_metrics(line)
            if metrics:
                category = get_category(current_execution, testcases_category)

                type_execution = metrics["type_testcase"]

                if category not in stats_by_category:
                    stats_by_category[category] = {}

                if type_execution not in stats_by_category[category]:
                    stats_by_category[category][type_execution] = {}

                if current_execution not in stats_by_category[category][type_execution]:
                    stats_by_category[category][type_execution][current_execution] = [
                    ]

                stats_by_category[category][type_execution][current_execution].append(
                    metrics)
            else:
                line = line \
                    .replace("Running ", "") \
                    .replace("...", "")

                current_execution = line.lower()

    plot_metrics(stats_by_category)


if __name__ == "__main__":
    main(*sys.argv[1:])

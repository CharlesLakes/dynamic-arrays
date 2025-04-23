import matplotlib.pyplot as plt
import re
import sys
import json


def extract_metrics(line):
    pattern = re.compile(
        # r"AC - T(\d+)_(.*) \| Wall: ([\d.]+)s \| CPU User: ([\d.]+)s \| CPU Sys: ([\d.]+)s \| Max Memory:(\d+)B"
        r"AC - T(\d+)_?(.*) \| Wall: ([\d.]+)s \| CPU User: ([\d.]+)s \| CPU Sys: ([\d.]+)s \| Heap: (\d+)B \| Stack: (\d+)B"
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


# Function to plot metrics for each execution


def plot_metrics(stats_by_category):
    for category in stats_by_category:
        for type_execution in stats_by_category[category]:
            fig, axs = plt.subplots(2, 2, figsize=(14, 8))

            i = 0

            for current_execution in stats_by_category[category][type_execution]:

                executions_data = stats_by_category[category][type_execution][current_execution]
                ns = [execution_data["n"]
                      for execution_data in executions_data]
                cpu_users = [execution_data["cpu_user"]
                             for execution_data in executions_data]

                x, y = i // 2, i % 2

                axs[x, y].plot(ns, cpu_users, marker='o', color='orange')
                axs[x, y].set_title(f"{current_execution} - CPU User Time")
                axs[x, y].set_xlabel("n")
                axs[x, y].set_ylabel("CPU User Time (s)")
                axs[x, y].set_xscale('log')
                axs[x, y].grid(True)

                i += 1

            plt.tight_layout()
            plt.savefig(f"{current_execution}_cpu_user.png")

    for category in stats_by_category:
        for type_execution in stats_by_category[category]:
            fig, axs = plt.subplots(2, 2, figsize=(14, 8))

            i = 0

            for current_execution in stats_by_category[category][type_execution]:

                executions_data = stats_by_category[category][type_execution][current_execution]
                ns = [execution_data["n"]
                      for execution_data in executions_data]
                cpu_users = [execution_data["heap"]
                             for execution_data in executions_data]

                x, y = i // 2, i % 2

                axs[x, y].plot(ns, cpu_users, marker='o', color='orange')
                axs[x, y].set_title(f"{current_execution} - CPU User Time")
                axs[x, y].set_xlabel("n")
                axs[x, y].set_ylabel("Heap (Bytes)")
                axs[x, y].set_xscale('log')
                axs[x, y].grid(True)

                i += 1

            plt.tight_layout()
            plt.savefig(f"{current_execution}_heap.png")


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

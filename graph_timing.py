import csv
from collections import defaultdict
from statistics import mean

import matplotlib.pyplot as plt

INPUT_FILE = "timing_results.csv"
OUTPUT_FILE = "timing_graph.png"

rows = []

with open(INPUT_FILE, newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        row["threads"] = int(row["threads"])
        row["run"] = int(row["run"])
        row["chunks"] = int(row["chunks"])
        row["time_ms"] = float(row["time_ms"])
        rows.append(row)

groups = defaultdict(list)

for row in rows:
    label = f"{row['algorithm']} / {row['threads']} threads"
    groups[label].append(row["time_ms"])

labels = list(groups.keys())
averages = [mean(groups[label]) for label in labels]

print("Average times:")

for label, avg in zip(labels, averages):
    print(f"{label}: {avg:.3f} ms")

plt.figure(figsize=(10, 6))
plt.bar(labels, averages)
plt.ylabel("Average time (ms)")
plt.title("Chunk Generation Timing")
plt.xticks(rotation=25, ha="right")
plt.tight_layout()
plt.savefig(OUTPUT_FILE)
plt.show()

print(f"Saved graph to {OUTPUT_FILE}")

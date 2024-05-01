from sys import argv
from os import path
import numpy as np
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt
import seaborn as sns
import util

# Disable interactive mode
plt.ioff()
sns.set_theme(font_scale=1.8)
plt.rcParams.update({'font.size': 20})

################################################
#               ARGUMENTS PARSING              #
################################################
# Parse the command line arguments
parsed_args = util.parse_arguments(
    [
        ("--in", "Sets the benchmark file to read", str),
        ("--out", "Sets the folder to output to", str),
    ],
    argv,
    "== Python tool to generate basic plots from a benchmark file =="
)
assert("--in" in parsed_args and "--out" in parsed_args)
assert(path.isfile(parsed_args["--in"]))
assert(path.isdir(parsed_args["--out"]))
input_path:str = parsed_args["--in"]
output_path:str = parsed_args["--out"]


################################################
#                 PLOT BENCHMARK               #
################################################
print(f"> Generating plots from file: {input_path}")
# Load data
rays = []
time_data = []
trace_data = []
util.parse_benchmark(input_path, rays, time_data, trace_data)
N = len(rays)

## Printing compute step times
times = util.flatten_list(time_data)
print(f"{len(times)} compute step times measured:")
print(f"Mean time: {np.mean(times)}")
print(f"Standard deviation: {np.std(times)}")
del times

## Printing full ray times
print(f"{N} ray times measured:")
ray_times = [sum(ray_times) for ray_times in time_data]
print(f"Mean time: {np.mean(ray_times)}")
print(f"Standard deviation: {np.std(ray_times)}")

## Printing hitting full ray times
ray_hitting_times = [ ray_times[i] for i in range(N) if util.point_in_bounds(trace_data[i][-1]) ]
print(f"{len(ray_hitting_times)} hitting ray times measured:")
print(f"Hitting frequency: {len(ray_hitting_times)/len(ray_times)}")
print(f"Mean time: {np.mean(ray_hitting_times)}")
print(f"Standard deviation: {np.std(ray_hitting_times)}")

## Plotting time function to vertical ray direction
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()

# Compute mean time for the vertical direction
x = []
y = []
for i,ray in enumerate(rays):
    x.append(float(ray[1][1]))# Only keep the y direction
    y.append(np.mean(time_data[i]))

ax.plot(x, y, 'o')

ax.set_xlabel('Vertical ray direction')
ax.set_ylabel('Mean compute time (µs)')
ax.grid(True)
ax.set_ylim(0, max(y)*1.1)
#ax.legend()
ax.set_title(
    "Mean compute time on vertical ray direction",
    loc='left',
    weight='bold'
)
fig.savefig(f"{output_path}/{path.basename(input_path)}-time_on_vert_dir.pdf", dpi=100, format='pdf')
plt.close(fig)
print("Saved 'Time by vertical ray direction' plot")

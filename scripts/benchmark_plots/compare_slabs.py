from sys import argv
from os import path
import resource
import gc
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
        ("--prefix", "Sets the benchmark file prefix to use", str),
        ("--out", "Sets the folder to output to", str),
    ],
    argv,
    "== Python tool to compare and plot slabs and marching slabs algorithm =="
)
assert "--prefix" in parsed_args and "--out" in parsed_args
assert path.isfile(f"{parsed_args["--prefix"]}slabs.txt")
assert path.isdir(parsed_args["--out"])
prefix_path:str = parsed_args["--prefix"]
output_path:str = parsed_args["--out"]

################################################
#                   DEFINES                    #
################################################
steps_values = np.arange(0.01,0.51,0.01)

################################################
#                LOAD EVERYTHING               #
################################################
rays_marching = [ [] for _ in steps_values ]
times_marching = [ [] for _ in steps_values ]
traces_marching = [ [] for _ in steps_values ]
# Load all the files 1 by 1
for step_index, step in enumerate(steps_values):
    filename = f"{prefix_path}marching_{("%.6f" % step)}.txt"
    print(f"Loading {filename}")
    util.parse_benchmark(filename, rays_marching[step_index], times_marching[step_index], traces_marching[step_index])
    print(f"Using {resource.getrusage(resource.RUSAGE_SELF).ru_maxrss}B")

# Check that all the rays are the same (direction and origin)
assert all(map(lambda ray: ray == rays_marching[0], rays_marching))

rays:list[tuple[tuple[float],tuple[float]]] = rays_marching[0] # [(origin, direction) for ray in rays]
del rays_marching
N = len(rays)

# Slabs file
rays_slabs = []
times_slabs = []
traces_slabs = []
util.parse_benchmark(f"{prefix_path}slabs.txt", rays_slabs, times_slabs, traces_slabs)
print(f"Loaded {prefix_path}slabs.txt")

# Check again that all the rays are the same
assert rays == rays_slabs
del rays_slabs


################################################
#             PLOT MARCHING ERRORS             #
################################################
## DATA
marching_errors_counts = [ 0 for _ in range(len(steps_values)) ]
hitting_rays_counts = [ 0 for _ in range(len(steps_values)) ]

for step_index in range(len(steps_values)):
    for i,trace in enumerate(traces_marching[step_index]):
        # if util.point_in_bounds(trace[-1]) and not util.point_in_bounds(traces_slabs[i][-1]):
        #     print(f"No intersection found for ray {i} yet marching algorithm stops")
        #     print(f"Ray {i} missed an intersection at step {steps_values[step_index]}")
        #     print(f"Ray: {rays[i]}")
        #     print(trace)
        #     print("---")
        #     print(traces_slabs[i])
        #     exit(71)

        if not util.point_in_bounds(traces_slabs[i][-1]):
            continue

        hitting_rays_counts[step_index] += 1

        if not util.equal_points(trace[-1], traces_slabs[i][-1]):
            marching_errors_counts[step_index] += 1
            # print(f"Ray {i} missed an intersection at step {steps_values[step_index]}")
            # print(f"Ray: {rays[i]}")
            # print(trace)
            # print("---")
            # print(traces_slabs[i])

## FIGURE
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, [ marching_errors_counts[i] / hitting_rays_counts[i] for i in range(len(steps_values)) ], 'o-')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Ratio of intersection misses over all ray hits')
ax.grid(True)
#ax.set_ylim(0, max(y)*1.1)
#ax.legend()
#ax.set_title(
#    "Marching step errors function to step size",
#    loc='left',
#    weight='bold'
#)
fig.savefig(f"{output_path}/{path.basename(prefix_path)}marching_errors.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'Marching step errors' plot")
del marching_errors_counts
gc.collect()

################################################
#              PLOT MEAN RAY TIMES             #
################################################
## DATA
# Marching
ray_times_marching = []
for step_index in range(len(steps_values)):
    ray_times_marching.append([sum(step_times) for step_times in times_marching[step_index]])
marching_quantiles_025, marching_median, marching_quantiles_075 = np.quantile(ray_times_marching, [0.25,0.5,0.75], axis=1)
# Slabs
slabs_median = [np.median([ sum(step_times) for step_times in times_slabs ]) ] * len(steps_values)# Constant list

## FIGURE
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, marching_median, 'o-', label='Marching Slabs')
ax.fill_between(steps_values, marching_quantiles_025, marching_quantiles_075, alpha=.3, linewidth=0)
ax.plot(steps_values, slabs_median, 'x-', label='Classical Slabs')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Median ray time (µs)')
ax.grid(True)
ax.legend()
#ax.set_title(
#    "Median ray shooting compute time function to step size",
#    loc='left',
#    weight='bold'
#)
fig.savefig(f"{output_path}/{path.basename(prefix_path)}median_ray_times.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'Median ray times' plot")
gc.collect()

################################################
#         PLOT HITTING MEAN RAY TIMES          #
################################################
## DATA
# Marching
hitting_ray_times_marching = []
for step_index in range(len(steps_values)):
    hitting_ray_times_marching.append(
        [ np.sum(times) for i,times in enumerate(ray_times_marching[step_index])
          if util.point_in_bounds(traces_marching[step_index][i][-1]) ])
marching_quantiles_025, marching_median, marching_quantiles_075 = [], [], []
for step_index in range(len(steps_values)):
    marching_quantiles_025.append(np.quantile(hitting_ray_times_marching[step_index], 0.25))
    marching_median.append(np.median(hitting_ray_times_marching[step_index]))
    marching_quantiles_075.append(np.quantile(hitting_ray_times_marching[step_index], 0.75))
# Slabs
slabs_median_ = [np.median(
    [ sum(step_times) for i,step_times in enumerate(times_slabs) if util.point_in_bounds(traces_slabs[i][-1]) ])] * len(steps_values)# Constant list

## FIGURE
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, marching_median, 'o-', label='Marching Slabs')
ax.fill_between(steps_values, marching_quantiles_025, marching_quantiles_075, alpha=.3, linewidth=0)
ax.plot(steps_values, slabs_median, 'x-', label='Classical Slabs')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Median ray time (µs)')
ax.grid(True)
ax.legend()
#ax.set_title(
#    "Median ray shooting compute time function to step size",
#    loc='left',
#    weight='bold'
#)
fig.savefig(f"{output_path}/{path.basename(prefix_path)}median_hitting_times.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'Median hitting ray times' plot")
gc.collect()

################################################
#         PLOT MISSING MEAN RAY TIMES          #
################################################
## DATA
missing_ray_times_marching = []
for step_index in range(len(steps_values)):
    missing_ray_times_marching.append(
        [ np.sum(times) for i,times in enumerate(ray_times_marching[step_index])
          if not util.point_in_bounds(traces_marching[step_index][i][-1]) ])
marching_quantiles_025, marching_median, marching_quantiles_075 = [], [], []
for step_index in range(len(steps_values)):
    marching_quantiles_025.append(np.quantile(missing_ray_times_marching[step_index], 0.25))
    marching_median.append(np.median(missing_ray_times_marching[step_index]))
    marching_quantiles_075.append(np.quantile(missing_ray_times_marching[step_index], 0.75))
# Slabs
slabs_median_ = [np.median(
    [ sum(step_times) for i,step_times in enumerate(times_slabs) if not util.point_in_bounds(traces_slabs[i][-1]) ])] * len(steps_values)# Constant list


## FIGURE
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, marching_median, 'o-', label='Marching Slabs')
ax.fill_between(steps_values, marching_quantiles_025, marching_quantiles_075, alpha=.3, linewidth=0)
ax.plot(steps_values, slabs_median, 'x-', label='Classical Slabs')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Median ray time (µs)')
ax.grid(True)
ax.legend()
#ax.set_title(
#    "Median ray shooting compute time function to step size",
#    loc='left',
#    weight='bold'
#)
fig.savefig(f"{output_path}/{path.basename(prefix_path)}median_missing_times.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'Median missing ray times' plot")
gc.collect()

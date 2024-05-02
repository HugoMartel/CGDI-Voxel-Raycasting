from sys import argv
from os import path
import resource
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
#                LOOK FOR MISSES               #
################################################
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

################################################
#             PLOT MARCHING ERRORS             #
################################################
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, [ marching_errors_counts[i] / hitting_rays_counts[i] for i in range(len(steps_values)) ], 'o-')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Ratio of intersection misses over all ray hits')
ax.grid(True)
#ax.set_ylim(0, max(y)*1.1)
#ax.legend()
ax.set_title(
    "Marching step errors function to step size",
    loc='left',
    weight='bold'
)
fig.savefig(f"{output_path}/{path.basename(prefix_path)}-marching_errors.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'Marching step errors' plot")

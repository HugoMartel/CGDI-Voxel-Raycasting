from sys import argv
from os import path
import numpy as np
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt
import seaborn as sns

# Disable interactive mode
plt.ioff()
sns.set_theme(font_scale=1.8)
plt.rcParams.update({'font.size': 20})

################################################
#               ARGUMENTS PARSING              #
################################################
in_path_prefix:str = ""
out_path:str = ""

def print_help() -> None:
    print("== Python tool to generate a plot from a benchmark file ==")
    print("Usage:")
    print("  --in=<Benchmark file input>      Sets the file to read")
    print("  --out=<output folder>      Sets the folder to output to")
    print("  --help                     Prints this text")


# Parse the command line arguments
for arg in argv[1:]:
    if arg == "--help":
        print_help()
        exit(0)
    elif arg.startswith("--in="):
        if len(arg) <= 5:
            print("Bad --in= argument, missing file path")
            print_help()
            exit(-1)

        file_loc_prefix = arg.split("=")[1]
        if path.isfile(file_loc_prefix+"slabs.txt"):
            in_path_prefix = file_loc_prefix
        else:
            print("No file found at the specified location: "+file_loc_prefix+"slabs.txt\nCheck if the provided prefix is correct")
            exit(-1)
    elif arg.startswith("--out="):
        if len(arg) <= 6:
            print("Bad --out= argument, missing file path")
            print_help()
            exit(-1)

        folder_loc = arg.split("=")[1]
        if path.isdir(folder_loc):
            out_path = folder_loc
        else:
            print("No folder found at the specified location: "+folder_loc)
            exit(-1)
    else:
        print("Unknown argument: "+arg)
        print_help()
        exit(-1)

################################################
#           UTIL FUNCTIONS & DEFINES           #
################################################
x_limit:tuple[float,float] = (0.,15.)
y_limit:tuple[float,float] = (0.,15.)
z_limit:tuple[float,float] = (0.,15.)
#steps_values = np.arange(0.01,0.51,0.01)
steps_values = np.arange(0.1,0.51,0.01)

def point_in_bounds(point) -> bool:
    return (point[0] >= x_limit[0] and point[0] <= x_limit[1]
      and point[1] >= y_limit[0] and point[1] <= y_limit[1]
        and point[2] >= z_limit[0] and point[2] <= z_limit[1])

def equal_points(p1, p2) -> bool:
    return abs(p1[0]-p2[0]) < 1e-4 and abs(p1[1]-p2[1]) < 1e-4 and abs(p1[2]-p2[2]) < 1e-4

################################################
#                LOAD EVERYTHING               #
################################################
rays_marching = [ () for _ in steps_values ]
times_marching = [ [] for _ in steps_values ]
traces_marching = [ [] for _ in steps_values ]
for step_index, step in enumerate(steps_values):
    filename = f"{in_path_prefix}marching_{("%.6f" % step)}.txt"

    print(f"Opening {filename}")
    with open(filename, 'r') as f:
        for i,line in enumerate(f):
            # line format:
            # ray origin;ray direction|trace 0;compute time;...;trace x;compute time;
            # x is not known since the ray is random and can hit or not
            ray, traces_times = line[:-2].split('|')# Remove trailing ;\n and splits ray and data

            # Parse the ray as being a 2tuple of points, points being a 3-tuple of floats
            rays_marching[step_index] = (( (float(x) for x in point) for point in ray.split(';') ))

            # Init new traces for this line
            times_marching[step_index].append([])
            traces_marching[step_index].append([])

            # Iterate on every trace point and compute time
            for j,tmp in enumerate(traces_times.split(';')):
                if j % 2 == 0:# Trace point
                    traces_marching[step_index][i].append(( float(x) for x in tmp.split(',') ))# create a double tuple by comprehension
                else:# Computing time in µs
                    times_marching[step_index][i].append(float(tmp[:-2]))# remove µs from the end
    print(f"Loaded {filename}")

# Check that all the rays are the same (direction and origin)
assert(all(map(lambda ray: ray == rays_marching[0], rays_marching.values())))

rays:list[tuple[tuple[float],tuple[float]]] = rays_marching[0] # [(origin, direction) for ray in rays]
del rays_marching
N = len(rays)

# Slabs file
rays_slabs = []
times_slabs = []
traces_slabs = []
with open(f"{in_path_prefix}slabs.txt", 'r') as f:
    for i,line in enumerate(f):
        # line format:
        # ray origin;ray direction|trace 0;compute time;...;trace x;compute time;
        # x is not known since the ray is random and can hit or not
        ray, traces_times = line[:-2].split('|')# Remove trailing ;\n and splits ray and data

        # Parse the ray as being a 2tuple of points, points being a 3-tuple of floats
        rays_slabs.append(( (float(x) for x in point) for point in ray.split(';') ))

        # Init new traces for this line
        times_slabs.append([])
        traces_slabs.append([])

        # Iterate on every trace point and compute time
        for j,tmp in enumerate(traces_times.split(';')):
            if j % 2 == 0:# Trace point
                traces_slabs[i].append(( float(x) for x in tmp.split(',') ))# create a double tuple by comprehension
            else:# Computing time in µs
                times_slabs[i].append(float(tmp[:-2]))# remove µs from the end
print(f"Loaded {in_path_prefix}slabs.txt")

# Check again that all the rays are the same
assert(rays == rays_slabs)
del rays_slabs

################################################
#                LOOK FOR MISSES               #
################################################
marching_errors_counts = [ 0 for _ in range(N) ]
for step_index in range(N):
    for i,trace in enumerate(traces_marching[step_index]):
        if not equal_points(trace[-1], traces_slabs[i][-1]):
            marching_errors_counts[step_index] += 1

################################################
#             PLOT MARCHING ERRORS             #
################################################
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()
ax.plot(steps_values, marching_errors_counts, 'o-')

ax.set_xlabel('Marching step size')
ax.set_ylabel('Number of intersection misses')
ax.grid(True)
#ax.set_ylim(0, max(y)*1.1)
#ax.legend()
ax.set_title(
    "Marching step errors function to step size",
    loc='left',
    weight='bold'
)
fig.savefig(out_path+"/"+path.basename(in_path_prefix)+"-marching_errors.pdf", dpi=100, format='pdf')
plt.close(fig)
print(f"Saved 'time on vertical ray direction' plot")

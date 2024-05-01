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

def flatten_list(l:list) -> list:
    flat_list = []
    for smol_l in l:
        flat_list += smol_l
    return flat_list

################################################
#               ARGUMENTS PARSING              #
################################################
input_path:str = ""
output_path:str = ""

help_ = """== Python tool to generate a plot from a benchmark file ==")
print("Usage:")
print("  --in=<Benchmark file input>      Sets the file to read")
print("  --out=<output folder>      Sets the folder to output to")
print("  --help                     Prints this text"""

# Parse command line arguments
for arg in argv[1:]:
    if arg == "--help":
        print(help_)
        exit(0)

    if arg.startswith("--in="):
        if len(arg) <= 5:
            print("Bad --in= argument, missing file path")
            print(help_)
            exit(-1)

        file_loc = arg.split("=")[1]
        if path.isfile(file_loc):
            in_path = file_loc
            continue

        print(f"No file found at the specified location: {file_loc}")
        exit(-1)

    if arg.startswith("--out="):
        if len(arg) <= 6:
            print("Bad --out= argument, missing file path")
            print(help_)
            exit(-1)

        folder_loc = arg.split("=")[1]
        if path.isdir(folder_loc):
            output_path = folder_loc
            continue

        print(f"No folder found at the specified location: {folder_loc}")
        exit(-1)

    print(f"Unknown argument: {arg}")
    print(help_)
    exit(-1)


################################################
#                 PLOT BENCHMARK               #
################################################

print(f"> Generating plots from file: {in_path}")

# Load data
rays = []
time_data = []
trace_data = []

with open(in_path, 'r') as f:
    # Line format:
    # ray origin;ray direction|point 0;compute time;...;point n;compute time;
    # n is not known since the ray is random and can hit or not
    for line in f:
        # Remove trailing ;\n and split ray and data
        ray, trace = line[:-2].split('|')
        rays.append(ray.split(';'))
        trace = trace.split(';')

        times = []
        points = []
        for i in range(stop=len(trace), step=2):
            # remove µs from the end
            times.append(float(trace[i+1][:-2]))
            points.append([float(x) for x in trace[i].split(',')])
        time_data.append(times)
        trace_data.append(points)
N = len(rays)

# Print time
times = [time for l in time_data for time in l]
print(f"{len(times)} compute times measured:")
print(f"Mean time: {np.mean(times)}")
print(f"Standard deviation: {np.std(times)}")

# Plotting time function to vertical ray direction
fig = plt.figure(figsize=(16, 9))
ax = fig.add_subplot()

# Compute mean time for each direction
x = []
y = []
for i, ray in enumerate(rays):
    x.append(float(ray[1].split(',')[1]))
    y.append(np.mean(time_data[i]))

ax.plot(x, y, 'o')

ax.set_xlabel("Vertical ray direction")
ax.set_ylabel("Mean compute time (µs)")
ax.grid(True)
ax.set_ylim(0, max(y)*1.1)
#ax.legend()
ax.set_title(
    "Mean compute time on vertical ray direction",
    loc='left',
    weight='bold'
)
fig.savefig(f"{output_path}/{path.basename(in_path)}-time_on_vert_dir.pdf", dpi=100, format='pdf')
plt.close(fig)
print("Saved 'time on vertical ray direction' plot")

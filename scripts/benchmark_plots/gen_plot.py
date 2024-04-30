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
in_path:str = ""
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

        file_loc = arg.split("=")[1]
        if path.isfile(file_loc):
            in_path = file_loc
        else:
            print("No file found at the specified location: "+file_loc)
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
#                 PLOT BENCHMARK               #
################################################
print("> Generating plots from file: "+in_path)
# Load data
rays = []
time_data = []
trace_data = []
with open(in_path, 'r') as f:
    for line in f:
        # line format:
        # ray origin;ray direction|trace 0;compute time;...;trace x;compute time;
        # x is not known since the ray is random and can hit or not
        ray , data = line[:-2].split('|')# Remove trailing ;\n and split ray and data
        rays.append(ray.split(';'))
        times_traces = data.split(';')
        tmp_time = []
        tmp_trace = []
        for i in range(0, len(times_traces), 2):
            tmp_time.append(float(times_traces[i+1][:-2]))# remove µs from the end
            tmp_trace.append([float(x) for x in times_traces[i].split(',') ])
        time_data.append(tmp_time)
        trace_data.append(tmp_trace)
N = len(rays)

## Printing time
times = flatten_list(time_data)
print(str(len(times))+" compute times measured:")
print("Mean time: ", np.mean(times))
print("Standard deviation: ", np.std(times))

## Plotting time function to vertical ray direction
fig = plt.figure(figsize=(16,9))
ax = fig.add_subplot()

# Compute mean time for each direction
x = []
y = []
for i,ray in enumerate(rays):
    x.append(float(ray[1].split(',')[1]))
    y.append(np.mean(time_data[i]))

ax.plot(x, y, 'o')

ax.set_xlabel('vertical ray direction')
ax.set_ylabel('mean compute time (µs)')
ax.grid(True)
ax.set_ylim(0, max(y)*1.1)
#ax.legend()
ax.set_title(
    "Mean compute time on vertical ray direction",
    loc='left',
    weight='bold'
)
fig.savefig(out_path+"/"+path.basename(in_path)+"-time_on_vert_dir.pdf", dpi=100, format='pdf')
plt.close(fig)
print("Saved 'time on vertical ray direction' plot")


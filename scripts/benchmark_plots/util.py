from os import path
import gc
from math import sqrt

class SceneBounds:
    x:tuple[float,float] = (0., 16.)
    y:tuple[float,float] = (0., 16.)
    z:tuple[float,float] = (0., 16.)


def point_in_bounds(point:tuple[float,float,float]) -> bool:
    return (point[0] > SceneBounds.x[0] and point[0] < SceneBounds.x[1]
      and point[1] > SceneBounds.y[0] and point[1] < SceneBounds.y[1]
        and point[2] > SceneBounds.z[0] and point[2] < SceneBounds.z[1])


error:float = 1e-5
def equal_points(p1:tuple[float,float,float], p2:tuple[float,float,float]) -> bool:
    return abs(p1[0]-p2[0]) < error and abs(p1[1]-p2[1]) < error and abs(p1[2]-p2[2]) < error
    # return sqrt((p1[0]-p2[0])**2 + (p1[1]-p2[1])**2 + (p1[2]-p2[2])**2) < error


def str_to_point(s:str) -> tuple[float,float,float]:
    return tuple(float(x) for x in s.split(','))


def flatten_list(l:list) -> list:
    flat_list = []
    for smol_l in l:
        flat_list += smol_l
    return flat_list


def parse_benchmark(filename:str, out_rays:list[tuple[float,float,float]],
                    out_times:list[float], out_traces:list[tuple[float,float,float]]) -> None:
    assert(path.exists(filename))
    with open(filename, 'r') as f:
        for line in f:
            # line format:
            # ray origin;ray direction|trace 0;compute time;...;trace x;compute time;
            # x is not known since the ray is random and can hit or not
            ray, traces_times = line[:-2].split('|')# Remove trailing ;\n and splits ray and data

            # Parse the ray as being a 2tuple of points, points being a 3-tuple of floats
            out_rays.append(tuple(str_to_point(point) for point in ray.split(';')))

            # Init new traces for this line
            out_times.append([])
            out_traces.append([])

            # Iterate on every trace point and compute time
            for j,tmp in enumerate(traces_times.split(';')):
                if j % 2 == 0:# Trace point
                    out_traces[-1].append(str_to_point(tmp))# create a double tuple by comprehension
                else:# Computing time in µs
                    out_times[-1].append(float(tmp))
    gc.collect()


def parse_arguments(options:list[tuple[str,str,type]], argv:list[str], help_header:str) -> dict[str,any]:
    skip_arg:bool = False
    output = {}
    argc = len(argv)

    for i,arg in enumerate(argv[1:]):# We skip the program name in argv
        # Check if we have to skip this arg
        if skip_arg:
            skip_arg = False
            continue

        # Check if the program asked for help
        if arg == "--help":
            print(help_header)
            for option in options:
                print(f"  {option[0]:<30}{option[1]}")
            print("  --help                        Prints this text")
            exit(0)

        not_found:bool = True
        for option in options:
            if arg != option[0]:
                continue
            if option[2] == None:
                output[arg] = True
            else:
                if argc == i+1:
                    print(f"Missing value for argument {options}")
                output[arg] = option[2](argv[i+2])# +2 since we removed the first arg
                skip_arg = True
            not_found = False
            break

        if not_found:
            print(f"Unknown argument {arg}")
            exit(-1)

    return output

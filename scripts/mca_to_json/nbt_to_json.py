from pynbt import NBTFile
from sys import argv
from os import path
import json

in_path:str = ""
out_path:str = ""
verbose:bool = False

def print_help() -> None:
    print("== Python tool to extract block informations from chunks to JSON ==")
    print("Usage:")
    print("  --in=<NBT file input>      Sets the file to read")
    print("  --out=<JSON file output>   Sets the file to output to")
    print("  --verbose                  Enables verbose output")
    print("  --help                     Prints this text")


# Parse the command line arguments
for arg in argv[1:]:
    if arg == "--help":
        print_help()
        exit(0)
    elif arg == "--verbose":
        verbose = True
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

        out_path = arg.split("=")[1]
    else:
        print("Unknown argument: "+arg)
        print_help()
        exit(-1)


# Open the NBT file
with open(in_path, 'rb') as io:
    nbt = NBTFile(io)
    # print(nbt.pretty()) #! DEBUG

    # JSON Object to output
    data = {}

    # https://minecraft.wiki/w/Chunk_format

    # xPos: X position of the chunk
    # zPos: Z position of the chunk
    # (in absolute chunks from world (x, z) origin, not relative to the region).
    # print('xPos', nbt['xPos'].value) #! DEBUG
    # print('zPos', nbt['zPos'].value) #! DEBUG
    data['xPos'] = nbt['xPos'].value
    data['zPos'] = nbt['zPos'].value
    # yPos: Lowest Y section position in the chunk (e.g. -4 in 1.18).
    # print('yPos', nbt['yPos'].value) #! DEBUG
    data['yPos'] = nbt['yPos'].value
    # X increases East, decreases West
    # Y increases upward, decreases downward
    # Z increases South, decreases North

    block_entities = []
    for be in nbt['block_entities']:
        tmp_be = {}
        tmp_be['id'] = be['id'].value
        tmp_be['keepPacked'] = be['keepPacked'].value
        tmp_be['x'] = be['x'].value
        tmp_be['y'] = be['y'].value
        tmp_be['z'] = be['z'].value
        block_entities.append(tmp_be)

    # Each tag is a section (also known as sub-chunk). All sections in the world's
    # height are present in this list, even those who are empty (filled with air).
    sections = []
    for tag in nbt['sections']:
        # print("----") #! DEBUG
        sections.append({})

        # The Y position of this section.
        # print('Y', tag['Y'].value) #! DEBUG
        sections[-1]['Y'] = tag['Y'].value

        block_states = tag['block_states']

        # Set of different block states used in this particular section.
        palette_list = []
        for block in block_states['palette']:
            palette_list.append({})
            palette_list[-1]['Name'] = block['Name'].value
            if 'Properties' in block:
                properties = {}
                for name, prop in block['Properties'].items():
                    properties[name] = prop.value
                palette_list[-1]['Properties'] = properties

        sections[-1]['palette'] = palette_list

        # A packed array of 4096 indices pointing to the palette,
        # stored in an array of 64-bit integers.
        # If only one block state is present in the palette, this field is not required
        # and the block fills the whole section.
        if 'data' in block_states:
            # print(list(block_states['data'].value)) #! DEBUG
            sections[-1]['data'] = list(block_states['data'].value)

    data['sections'] = sections

    # print(data) #! DEBUG

    # Write JSON to file
    with open(out_path, 'w', encoding='utf-8') as f:
        json.dump(data, f, ensure_ascii=False, indent=4)


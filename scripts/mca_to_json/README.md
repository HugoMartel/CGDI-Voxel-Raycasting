# Extracting blocks from chunk .mca file

We used [MCA2NBT](https://github.com/HubTou/MCA2NBT) C library to convert a `.mca` file to 32x32 chunk `.nbt` files

Then, we convert `.nbt` files to `.json` files only containing information on the blocks.

## Dependencies

The code has only been tested on Linux. You will need the following packages to be able to run the code:

- `make`

- `curl`

- python pip module `pynbt`

On Ubuntu, you can install them by running
```sh
sudo apt install make curl
python -m pip install pynbt
```

## Usage

Then to run the converter, do
```sh
./mca_to_json.sh <MCA file path>
```

*an example region MCA file of a superflat world is available in `regions/superflat.mca`*


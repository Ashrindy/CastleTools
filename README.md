# Castle Tools

A library `castle-lib` and CLI tool `castle-lib-tool`.

## Features

```
FORMATS
.rda .RDA             Archive file format.
COMMANDS
--help          -h                                                           Shows a description of every command.
--version       -v                                                           Shows the version of the tool.
--search        -s    {search-value} {RDA-filepath}                          Searches for a file in a specified RDA file.
--search-batch  -sb   {search-value} {directory-of-RDA}                      Searches a directory of RDA's for a file.
--replace-file  -r    {search-value} {RDA-filepath} {replacement-filepath}   Replaces a file in a specified RDA file.
```

## Building

### Windows

Requires [MinGW](https://github.com/niXman/mingw-builds-binaries) and [CMake](https://cmake.org/)

```
$ cmake .. -B build --preset mingw-windows-release
$ cd build
$ mingw32-make -j 8
```

### Linux

Requires GCC and CMake

```
$ cmake .. -B build --preset gcc-linux-release
$ cd build
$ make -j 8
```

# Disclaimer

Castle Tools is not affiliated with or endorsed by the game's rights holders.

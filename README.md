# ini-cpp

ini-cpp is a lightweight ".ini" file parser and emitter written in modern C++17. It provides an easy-to-use interface for reading and writing configuration files in the INI file format.

## Features

- Parsing and reading ".ini" files into a convenient data structure.
- Accessing and modifying sections, keys, and values programmatically.
- Emitting and writing configuration data back to ".ini" files.
- Error handling for malformed or invalid INI files.

## Requirements

- C++17-compatible compiler

## Usage

To use ini-cpp in your project, follow these steps:

1. Clone or download the ini-cpp repository. 
```sh
git clone https://github.com/Specyy/ini-cpp.git
```
2. Navigate into the source directory, create build folder and run `CMake`:
```sh
cd ini-cpp
mkdir build
cd build
cmake .. [-G generator] [-DINICPP_TEST=ON|OFF]
```

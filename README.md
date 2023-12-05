# SJSU-CS-149 Project

## Goals

To simulating five process management functions:
- Process creation
- Replacing the current process image with a new process image
- Process state transition
- Process scheduling
- Context switching

## Building

### CLion
Open the project in CLion with CMake 3.26 or higher.

### CMake

Make sure `cmake` and `ninja` are installed and available in your PATH.

To setup the project, run the following command:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B cmake-build-debug
```

To build the project, run the following command:
```bash
cmake --build cmake-build-debug --target SJSU_CS_149 -j 10
```

Then you'll find the executable in `cmake-build-debug/SJSU_CS_149`.

## Running

The easiest way to run a set of our tests is as follows:

```bash
./SJSU_CS_149 <optional:path/to/test/directory> <optional:path/to/commands>
```

Both arguments are optional. If no arguments are provided, the program will run the tests in the current working 
directory and the commands will be read from STDIN.

## Authors

- Noah Cardoza
- Raul Garcia
- Joshua Medina
- Kevin Campbell
# SJSU-CS-149
Simulating five process management functions: process creation, replacing the current process image with
a new process image, process state transition, process scheduling, and context switching.

## How to run

Open the project in CLion with CMake 3.26 or higher.

The easiest way to run a set of our tests is as follows:

```bash
./SJSU_CS_149 <optional:path/to/test/directory> <optional:path/to/commands>
```

Both arguments are optional. If no arguments are provided, the program will run the tests in the current working 
directory and the commands will be read from STDIN.
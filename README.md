# Ziki

_Repository for code written for 2017 Embedded Systems project_

## Introduction
These files are designed to compile from the command line provided the correct tools are installed.
The structure here started from [this repository](https://github.com/wangyeee/STM32F4-FreeRTOS) but
some stylistic changes have been made (such as changing directory names to lowercase where possible).
Please skim through [Makefile](Makefile) and [main.c](main.c) as they will provide perspective in the sections below.

To get set up on a new machine:
1. Install [stlink](https://github.com/texane/stlink/), this facilitates communication with the micro.
2. Make sure you have the [ARM GNU toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) installed.
3. Clone repo, making sure the `TOOLCHAIN_ROOT` variable in [Makefile](Makefile) is appropriate for
   your machine (I suggest softlink otherwise so we have a consistent Makefile in the repo). 

## Testing
To install on the micro run `make flash` after building the source with `make`.

In order to debug with GDB, the procedure is similar to last year. First type `st-util` into a
terminal, i.e.
```
$ st-util
st-util 1.4.0-6-g7b7a5c1
2017-10-03T20:29:57 INFO src/common.c: Loading device parameters....
2017-10-03T20:29:57 INFO src/common.c: Device connected is: F4 device, id 0x10076413
2017-10-03T20:29:57 INFO src/common.c: SRAM size: 0x30000 bytes (192 KiB), Flash: 0x100000 bytes
(1024 KiB) in pages of 16384 bytes
2017-10-03T20:29:57 INFO src/gdbserver/gdb-server.c: Chip ID is 00000413, Core ID is  2ba01477.
2017-10-03T20:29:57 INFO src/gdbserver/gdb-server.c: Listening at *:4242...
```
Taking note of the port number, `:4242` in this case, navigate to the project root folder in a
separate terminal then run
```
$ arm-none-eabi-gdb binary/ziki.elf
tar rem :4242
b main
continue
```
You can use all the commands from last year (like breakpoints, `-tui`) as appropriate.

## Project folder structure
The project folder is designed to allow for modular construction of FreeRTOS tasks.
Hence the basic idea is that [main.c](main.c) sets up FreeRTOS, which launches the various
independent tasks in [src](src/).

The [config](config/) folder contains library files describing the system at a software level, while
[hardware](hardware/) has the basic assembly necessary to launch into C. Finally [lib](lib/)
contains external libraries being used, and [util](util/) is for other important files (like the
linker description).

### Adding a task
To make a new task, first edit [main.c](main.c), mimicking the code for existing tasks. Add a line
in [ziki.h](src/ziki.h) with the entry-point for the task, then make the files needed within the
[src](src/) directory. Ensure that these are added to [Makefile](Makefile).

### Including libraries
It is worth noting that the project [Makefile](Makefile) was very carefully designed so that library
files are included as necessary, so you should be able to include header files from anywhere in the
project folders.

### Binaries
The other two folders in this directory are [binary](binary/) and [build](build/). The former of
these contains the output `ziki.elf` file, while the latter contains object files from the build.
Accordingly, files within these folders are ignored to avoid cluttering the repo.

### Ignored files
Please add any OS/IDE/editor specific files into an [ignore file](.gitignore) so that only the files
related to the project are in the repo.

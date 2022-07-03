# game-emu

[![game-emu](https://github.com/Dudejoe870/game-emu/actions/workflows/game-emu.yml/badge.svg)](https://github.com/Dudejoe870/game-emu/actions/workflows/game-emu.yml)

A cross-platform multi-system emulator / library
> Note: Heavily WIP

# Folder Structure

| Folder              | Description                                                                                                                                                        |
|---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| gameemu-cli        | A CLI UI for loading, running, and debugging game-emu cores with a runtime debugging CLI.                                                                          |
| gameemu-core-*      | A game-emu core that is linked as a dynamic library, then loaded at runtime, invoked using a common API.                                                           |
| gameemu-common     | The main folder of the project, this is the main library that all cores and UIs are built off of. Contains general utilities for making creating Emulators easier. |

# Cores

| Core               | Description                                                                         |
|--------------------|-------------------------------------------------------------------------------------|
| gameemu-core-gb    | A Core emulating the Gameboy                                                        |
| gameemu-core-sm83  | A Core emulating the Sharp SM83 found on the Gameboy                                |

More cores are to come, currently it is very WIP.

If you want to contribute a Core that'd be greatly appreciated!

# Pre-built Binaries

Currently this in development so it isn't really recommended to run the 
software if you don't intend to use the source code.

Nevertheless there are Binaries that get built every commit to the main branch,
just go to the ``Actions`` tab and click on the commit you want to download,
if you scroll down there should be ZIP files you can download.
(If there aren't that means the commit is too old, 
this can happen even on the newest commit if there hasn't been 
one in the while, so in that case you'll need to build from source).

> Note: It is NOT recommended to use the Linux build as it could heavily 
depend on what version of the operating system you are using. For example, 
the Ubuntu build won't work on Arch, due to LibC being a different version.
It also makes uninstalling a lot easier if you compile from source.

Just download the correct one for your operating system and architecture, 
and run the software. (Currently only supports x86-64, 
ARM support might come in the future most notably for Raspberry Pi / Android support
\+ perhaps even other architectures like PowerPC for porting 
to older consoles like the Wii / Xbox360)

# Building

## Dependencies

Due to most of the dependencies being included as submodules, 
there aren't actually really any to download 
other than CMake and a general building environment 
(MSVC, Clang, GCC) (Clang is untested but should(?) work)

## CMake Options

| Option             | Description                                                                                                                                                                                                                                                                                                |
|--------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| CMAKE_BUILD_TYPE   | Set this to either "Debug", "Release", or "RelWithDebInfo" (Release with Debug Info) to compile with that configuration.                                                                                                                                                                                   |
| GAME_EMU_CORE_LIST | A semicolon separated list of Cores you'd like to build (or alternatively can be "all"), the list can contain any of the core names listed above. This will automatically build the dependencies needed for each Core, so it is only required to list the system cores you want and the rest is automatic. |

You can use any of these options by passing ```-D(option)=(value)``` to CMake on the command line.

## Windows

In Windows it's recommended to use Visual Studio 2022 with the CMake support installed.

Just open the cloned repository as an external project, 
set your build configuration and build.

From there to run it you can go to 
Build > Install game-emu, that will put the executable in 
out/install/bin with the required DLLs, you can run the EXE from there.

## Linux

Under Linux it's also pretty straight forward, just have CMake installed, 
and GCC / Clang (again untested but should probably work) + either Ninja, 
or Make depending on which CMake generator you want to use (Ninja recommended as it's faster).

To build the project simply do
```bash
cmake -S ./ -B out/build -G Ninja
```
Then go into out/build and type
```bash
ninja
```
to build the software.

To run the software you can install it using
```bash
sudo cmake --install . [--prefix (install prefix)]
sudo ldconfig
```

You can specify an install prefix if you don't want to install it to the normal directory (usually /usr/local (make sure it's in your /etc/ld.so.conf!))

Once it's in your path you can now use ```gameemu-cli``` to run the software.
> Note: If you specified a custom install prefix, 
your LD_LIBRARY_PATH variable will have to 
include the lib subdirectory in the install path. 
Or you can add it to your /etc/ld.so.conf config.

To uninstall simply run
```bash
sudo ninja uninstall
```
from the build folder.
> Note: Doesn't get rid of all the subdirectories for security reasons 
(One misstep and we could accidentally delete your entire root drive!)

# Contribution

If you want to contribute, be my guest! I'd love to see what other people have to bring, 
even if it's just cleaning up some of my more... interesting code. 
(Hopefully it's not too bad!)

Currently the main library is what needs the most work
(the Cores that are currently being built are mostly 
just a testing ground for the library at the moment)
So any contributions to that (fixing up things, or 
adding new features, etc.) would be greatly appreciated!

# Issues

If you've read some of the code and have some suggestions but don't have time to 
contribute, opening an issue with any suggestions would be greatly appreciated as well!

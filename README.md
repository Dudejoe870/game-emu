# game-emu

A cross-platform multi-system emulator / library
> Note: Heavily WIP

# Folder Structure

| Folder              | Description                                                                                                                                                        |
|---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| game-emu-cli        | A CLI UI for loading, running, and debugging game-emu cores with a runtime debugging CLI.                                                                          |
| gameemu-core-*      | A game-emu core that is linked as a dynamic library, then loaded at runtime, invoked using a common API.                                                           |
| game-emu-common     | The main folder of the project, this is the main library that all cores and UIs are built off of. Contains general utilities for making creating Emulators easier. |

# Cores

| Core               | Description                                                                         |
|--------------------|-------------------------------------------------------------------------------------|
| gameemu-core-gb    | A Core emulating the Gameboy                                                        |
| gameemu-core-gbz80 | A Core emulating the specific Z80 found on the Gameboy (differs from a regular Z80) |

More cores are to come, currently it is very WIP.

If you want to contribute a Core that'd be greatly appreciated!

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
```

You can specify an install prefix if you don't want to install it to the normal directory (usually /usr/local (make sure it's in your /etc/ld.so.conf!))

Once it's in your path you can now use ```game-emu-cli``` to run the software.
> Note: If you specified a custom install prefix, 
your LD_LIBRARY_PATH variable will have to 
include the lib subdirectory in the install path. 
Or you can add it to your /etc/ld.so.conf config.

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

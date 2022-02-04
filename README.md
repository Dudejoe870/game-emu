# game-emu
A cross-platform multi-system emulator / library
> Note: Heavily WIP

# Folder Structure

| Folder              | Description                                                                                                                                                        |
|---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| game-emu-cli        | A CLI UI for loading, running, and debugging game-emu cores with a runtime debugging CLI.                                                                          |
| gameemu-core-*      | A game-emu core that is linked as a dynamic library, then loaded at runtime, invoked using a common API.                                                           |
| libgame-emu-common | The main folder of the project, this is the main library that all cores and UIs are built off of. Contains general utilities for making creating Emulators easier. |

# Cores

| Core  | Description                                                                         |
|-------|-------------------------------------------------------------------------------------|
| gb    | A Core emulating the Gameboy                                                        |
| gbz80 | A Core emulating the specific Z80 found on the Gameboy (differs from a regular Z80) |

More cores are to come, currently it is very WIP.

If you want to contribute a Core that'd be greatly appreciated!

# Building

## Dependencies
Due to most of the dependencies being included as submodules, 
there aren't actually really any to download 
other than CMake and a general building environment 
(MSVC, Clang, GCC) (Clang is untested but should(?) work)

## Windows

In Windows it's recommended to use Visual Studio 2022 with the CMake support installed.

Just open the cloned repository as an external project, 
set your build configuration and go.
> Note: After configuring for the first time, you may want to go up to
Project > Configure Cache to configure for a 
second time because sometimes some of the LLVM variables 
don't correctly get applied the first time
(Also do this if you delete the cache and do a complete reconfigure)

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

Simple as that, you now have built binaries.

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

## About
An NES emulator writting in C++. Features include:
- Emulation of the 6502 processor, including all official opcodes as well as interruption handling.
- Emulation of the PPU (picture processing unit) and its per cycle behaviour, calculating the pixel data required for rendering.
- Decoding of the iNES format to allow .nes files to be run.
Currently there is only support for mapper 0 games - the next step will be emulating other common mappers.

## Dependencies
This emulator runs on Linux using the SDL2 library for video output.

## Screenshots
![Donkey Kong](http://i.imgur.com/X4o7BOX.jpg)
![Mario Bros.](https://i.imgur.com/WUUznoJ.jpg)

## Usage
Using the emulator currently requires editing src/main.cpp to set the file location of an NES ROM file.
Provided the ROM does not require a mapper, the emulator should be able to run it smoothly.
Controls are currently fixed, with the following mappings.
```
-----------------------
      key        button
-----------------------
        Z             A
        X             B
Backspace        Select
   Return         Start
       Up            Up
     Down          Down
     Left          Left
    Right         Right
```

Mategame Software Development Kit
============

Software development kit and platform simulator for the Mategame.

The Mategame is an open software and open hardware kit based initially in [Hackvision](http://nootropicdesign.com/hackvision/games.html), [Arduino](http://arduino.cc/) and [TVout](http://code.google.com/p/arduino-tvout/). We aim to make it possible to build everything, from coding to assembling in a DIY way manner, in your "kitchen table" as it were.

**Official Project Description**: <http://matehackers.org/doku.php?id=video_game>

## Example source code

There are many examples in the `examples` folder. Most of them are taken from [here](http://nootropicdesign.com/hackvision/games.html), but we'll be putting up some games and utility libraries.

You can look at the source code and the makefiles to get started on your own projects.

## Dependencies

  * SDL and SDLMixer
  * Arduino IDE and avr toolchain
  * arduino-mk (if you want to use 'make upload' your projects into your Arduino board)

## Building your own code

You'll have to config some stuff to get it going, at least setting the `TARGET` variable as the name of your '.ino' (or '.pde') file.

See the `Makefile` on the root folder or see each project in the `examples` directory to get a sense for it.

There are two commands provided by this `Makefile`:

    make mock

        Compiles the sketch into a x86 program using SDL for graphics and sound

    make upload

        Compiles the skecth for the Arduino and uploads it

## Hardware schematics

Soon

## Hardware Peripherals

Soon
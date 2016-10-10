# wanglandau
This is a C++11 implementation of the Wang-Landau algorithm applied to a NxN Ising system wrapped on a torus with nearest neighbours interactions.

## Abstract
The aim of this work is to reproduce the results of Wang, Tsai and Exler [1] by using the Wang-Landau algorithm on a 2D n × n lattice with periodic boundary conditions and nearest neighbour interactions. The Wang-Landau approach provides us with a convenient way to obtain the Density of States for a wide range of systems, thus providing (in principle) the ability to compute many thermodynamic quantities at any temperature.

## Contents

This repository is organized as follows:

- _build.sh_: generic build script. When invoked it will simply create a build directory, configure and run cmake from within that directory and exit;
- **doc** folder: includes the documentation (to be read by the toolchain, and modified by humans) about the build configuration;
- **include** folder: includes the external libraries and will contain the generated 'config.h' file;
- **report** folder: includes a full report of my tests alongside its latex source and some script to rebuild the whole;
- **scripts** folder: includes some utilities such as the script _collect.sh_ which parallelize multiple instantiations of the program with a pre-determinated set of parameters;
- **src** folder: contains the actual source for the main executable.

1. The root folder contains a copy of the GNU General Public License v2(1991) and the useful "Analysis.nb" notebook file. Wolfram Mathematica has been an invaluable tool for the computation of the exact solution to the problem and its comparision with the genereted data sets.
2. The "wanglandau" folder contains all the sources along with the cmake configuration files, some bash utilities and the manpage "wl.1";
3. The "report" folder contains the written report that documents the results of my tests along its latex source and some building scripts;
4. Since I didn't ask for permission to distribute them, the scientific papers and lecture notes on which this work has been based are not included. The interested reader may look at the bibliography and find all the relevant information.

## Coding style

I only wish to stress that almost all the code of this program was taken out from a generic simulation library of my own writing (way too young to have its own repository but growing (not so) rapidly). This is the reason why the code may seem a bit cluttered and some otherwise unexplainable choices have been made when deciding its overall structure.

## Build and install

I _weakly_ recommend to simply launch the script "build.sh" from within its enclosing folder. Obviously, anyone is free to choose is own toolchain and its only real purpose is to _cd_ to the "build" directory and launch cmake from there, thus keeping the original project clean and organized.

Some important configuration options (like the dimensionality of the simulation box and the pseudo-random number generator to be used) can be modified from within 'CMakeLists.txt' along with many other build options. The same applies for the choice of the compiler and the optimization level to use.

The build script will **NOT** install any file in any location outside the current working directory. I feel like no one will ever really need nor want to have a program like this amongst his system binaries and its much better that it stays where it belongs: next to the source code, the data directory, the documentation and all of the other relevant information.

All this information and more is available in the written report.

### Any constructive criticism or suggestion potentially leading to improvement of this software will be greatly appreciated!
You can send some to gian.salvato@gmail.com or through my new [GitHub account](https://github.com/gianlucasalvato) if you wish!

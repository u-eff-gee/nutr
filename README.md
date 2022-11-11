# **nutr**: new utr

A [Geant4](https://geant4.cern.ch) [1] simulation of experiments in the Upstream Target Room (UTR) at the [High Intensity γ-ray Source (HIγS)]() [2] of the Triangle Universities Nuclear Laboratory.

## Table of Contents

1. [Description](#1.-Description)

2. [Build](#2.-Build)

    2.1 [Prerequisites](#2.1-Prerequisites)

    2.2 [Build Variables](#2.2-Build-Variables)

3. [Development](#3.-Development)

    3.1 [Code Formatting](#3.1-Code-Formatting)

    3.2 [Docker](#3.2-Docker)

4. [License](#4.-License)

5. [Acknowledgments](#5.-Acknowledgments)

6. [References](#6.-References)

## 1. Description

The main program, user actions, and visualization macros of `nutr` are based on example B2a of Geant4 (`$G4_INSTALL_DIR/share/Geant4-10.6.1/examples/basic/B2/B2a`).
An analysis manager was added, which is based on the examples AnaEx* of Geant4 (`$G4_INSTALL_DIR/share/Geant4-10.6.1/examples/extended/analysis/AnaEx*`).
Some code has been adapted from a previous simulation of the UTR ([utr](https://github.com/uga-uga/utr) [3]).
`nutr` can employ the [alpaca](https://github.com/uga-uga/alpaca) library to generate gamma-ray cascades with realistic direction-direction and polarization-direction correlations.

## 2. Build

### 2.1 Prerequisites

* [Geant4 11.0 and its prerequisites](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html). It is assumed that Geant4 is installed in `G4_INSTALL_DIR`. If the low-energy nuclear data (LEND) for Geant4 have been downloaded and the `G4LENDDATA` environment variable has been set, then `nutr` can automatically use them.
* A compiler that supports the [C++20](https://en.cppreference.com/w/cpp/20) standard (`nutr` uses 'designated initializers' to initialize detector properties in a transparent way, and that is a C++20 feature. See `${NUTR_SOURCE_DIR}/include/detectors/HPGe_Collection.hh`, for example).
* `boost.program_options` from the [boost](https://www.boost.org/) C++ libraries
* [ROOT 6](https://root.cern.ch/) is optional, but since the output is written in the ROOT format by default, it is highly recommended.
* [Doxygen](http://www.doxygen.nl/index.html) and its [requirements for typesetting LaTeX](http://www.doxygen.nl/manual/formulas.html) formulas (optional)
* [alpaca](https://github.com/uga-uga/alpaca) (version >= 0.9.0) to use the `angcorr` primary generator (optional). The `alpaca` library must have been installed (`cmake --install .`) for `nutr` to be able to find it.

In the following, the directory where the top-level `CMakeLists.txt` is located in a downloaded version of `nutr` is denoted as `NUTR_SOURCE_DIR`.
In a build directory `NUTR_BUILD_DIR`, execute:

    $ cmake $NUTR_SOURCE_DIR
    $ cmake --build .

This creates an executable `nutr_GEOMETRY` in `NUTR_BUILD_DIR/GEOMETRY` for each detector geometry `GEOMETRY` that was selected via `CMake` build variable (see 2.2 [Build Variables](#2.2-Build-Variables)).
By default, executables for all geometries will be built.

If `nutr_GEOMETRY` is called without any command-line options, it will try to launch the interactive visualization

In most cases, a user will want to supply a macro file `MACRO` to the simulation, which is done by typing:

    $ nutr_GEOMETRY --macro MACRO

For an overview of all available command-line options, execute

    $ nutr_GEOMETRY --help

Alternatively, it is also possible to supply commands using a pipe:

    $ cat MACRO | nutr_GEOMETRY

### 2.2 Build Variables

After the first build step, several `CMake` build variables will be available for a customization of the build.
Besides the usual Geant4 build variables, `nutr` provides the following options:

* `BUILD_DOCUMENTATION`: Create the code documentation using Doxygen (default: OFF).
* `PRIMARY_GENERATOR_DIR`: Select directory in `$NUTR_SOURCE_DIR/src/fundamentals/primary_generator` that contains the desired primary generator Possible choices: `gps` (default), `angcorr`.
* `PRODUCTION_CUT_LOW_KEV`: Set the lower energy limit of the production cut for gammas, electrons/positrons and protons in keV (default: "0.99", i.e. use default production cut of `G4EmLivermorePolarizedPhysics`). A straightforward way to view the current production cuts is the `/run/particle/dumpCutValues` macro command.
* `SENSITIVE_DETECTOR_DIR`: Select directory in `$NUTR_SOURCE_DIR/src/sensitive_detector` that contains the desired sensitive detector. Possible choices: `edep`, `event` (default), `flux`, `tracker`.
* `UPDATE_FREQUENCY`: Determine the number of events since the last update after which a new update about the progress of the simulation is printed on the command line (default: 10000).
* `USE_HADRON_PHYSICS`: Include hadron physics lists (default: ON). Excluding hadron physics can speed up the startup of the simulation. This is useful, for example, when a user only wants to visualize the geometry. It might speed up the actual simulation as well, but, of course, sometimes hadron interactions cannot be neglected.
* `WITH_GEANT4_UIVIS`: Build `nutr` with Geant4 UI and Vis drivers (default: ON).

In addition, there is an option

* `GEOMETRY_*`: Build the given geometry (default: ON).

for each implemented geometry.

## 3. Development

### 3.1 Code Formatting

All code (all `.hh` and `.cc` files) of `nutr` is formatted using the tool [clang-format](https://clang.llvm.org/docs/ClangFormat.html).
The standard "LLVM" format option is used without any modifications for simplicity.
This style is defined in `NUTR_SOURCE_DIR/.clang-format`.

In order to simplify the code formatting, a [pre-commit](https://pre-commit.com/) hook has been defined in `NUTR_SOURCE_DIR/.pre-commit-config.yaml`.
The pre-commit formatting can be applied manually by typing

    $ pre-commit run --all-files

in the source directory `NUTR_SOURCE_DIR`.

### 3.2 Docker

A [docker](https://www.docker.com/) can be built from the source code using `NUTR_SOURCE_DIR/Dockerfile`.
At the moment, the build process in the `Dockerfile` includes downloading `Geant4` from its public git repository and building it from scratch.
This is by far the most time-consuming part of the containerization.
The docker build can be triggered by typing 

    $ docker build .

in `NUTR_SOURCE_DIR` (i.e. in the directory where the `Dockerfile` is).
Besides the system files of the `ubuntu` operating system on which the docker image is based, it contains the following directories:

* `/opt`: `Geant4` source code (`/opt/geant4`) and build directory (`/opt/geant4/build`).
* `/work`: `nutr` source code (`/work` is equivalent to `NUTR_SOURCE_DIR`) and build directory (`/work/build`)
* `/output`: Directory for output of the `nutr` simulation, intended to be mounted on the local machine using the `-v` or `--mount` flags of `docker`. See below for instructions on how to mount it.

In order to run an executable `nutr_GEOMETRY` with a macro file `MACRO` (at the moment, macro file must be in the docker image) from an image `IMAGE` and store the output in a local directory `LOCAL_OUTPUT_DIR` (if no local output directory is mounted, the simulation results will be lost on exit), execute

    $ docker run -v LOCAL_OUTPUT_DIR:/output IMAGE /work/build/GEOMETRY/nutr_GEOMETRY --macro MACRO

## 4. License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).

This product includes software developed by Members of the Geant4 Collaboration [http://cern.ch/geant4](http://cern.ch/geant4).

Copyright (C) 2020-2022 Udo Friman-Gayer (udo.friman-gayer@duke.edu) and Oliver Papst (opapst@ikp.tu-darmstadt.de)

## 5. Acknowledgments

The authors would like to thank Ethan McConnell for help with implementing geometry and Jörn Kleemann for helpful discussions.

## 6. References

[1] S. Agostinelli *et al.*, “GEANT4 - a simulation toolkit”, Nucl. Inst. Meth. A **506**, 250-303 (2003). [`doi:10.1016/S0168-9002(03)01368-8`](https://doi.org/10.1016/S0168-9002(03)01368-8); J. Allison *et al.*, “GEANT4 developments and applications”, IEEE T. Nucl. Sci., **53**, 270-278 (2006). [`doi:10.1109/TNS.2006.869826`](https://doi.org/10.1109/TNS.2006.869826); J. Allison *et al.*, “Recent developments in GEANT4”, Nucl. Inst. Meth. A **835**, 186-225 (2016). [`doi:10.1016/j.nima.2016.06.125`](https://doi.org/10.1016/j.nima.2016.06.125)

[2] H. R. Weller *et al.*, “Research opportunities at the upgraded HIγS facility”, Prog. Part. Nucl. Phys. **62**, 257-303 (2009). [`doi:10.1016/j.ppnp.2008.07.001`](https://doi.org/10.1016/j.ppnp.2008.07.001)

[3] U. Friman-Gayer, J. Kleemann, and O. Papst, “GEANT4 simulation of the Upstream Target Room (UTR) at the HIγS facility“ (2019) [`doi:10.5281/zenodo.3430154`](https://doi.org/10.5281/zenodo.3430154)

See also `NUTR_SOURCE_DIR/bibliography.bib`.

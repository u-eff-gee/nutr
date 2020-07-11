# **nutr**: new utr

This is a [Geant4](https://geant4.cern.ch) [1] simulation of experiments in the Upstream Target Room (UTR) at the [High Intensity γ-ray Source (HIγS)]() [2] of the Triangle Universities Nuclear Laboratory.

## Table of Contents

1. [Description](#1.-Description)

2. [Build](#2.-Build)

    2.1 [Prerequisites](#2.1-Prerequisites)

    2.2 [Build Variables](#2.2-Build-Variables)

3. [License](#3.-License)

4. [Acknowledgements](#4.-Acknowledgements)

5. [References](#5.-References)

## 1. Description

The main program, user actions, and visualization macros of `nutr` are based on example B2a of Geant4 (`$G4_INSTALL_DIR/share/Geant4-10.6.1/examples/basic/B2/B2a`).
An analysis manager was added, which is based on the examples AnaEx* of Geant4 (`$G4_INSTALL_DIR/share/Geant4-10.6.1/examples/extended/analysis/AnaEx*`).
Some code has been adapted from a previous simulation of the UTR ([utr](https://github.com/uga-uga/utr) [3]).

## 2. Build

### 2.1 Prerequisites

* [Geant4 10.6 and its prerequisites](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html). It is assumed that Geant4 is installed in `G4_INSTALL_DIR`.
* A compiler that supports the [C++14](https://en.cppreference.com/w/cpp/14) standard.
* [GNU Scientific Library (GSL)](https://www.gnu.org/software/gsl/)
* [ROOT 6](https://root.cern.ch/) is optional for building some tests of `nutr`. However, since the output is written in the ROOT format by default, it is highly recommended.
* [Doxygen](http://www.doxygen.nl/index.html) and its [requirements for typesetting LaTeX](http://www.doxygen.nl/manual/formulas.html) formulas (optional) 
* [python3](https://www.python.org/) to import the angular correlation function wrappers in python code, and the [matplotlib](https://matplotlib.org/) and [numpy](https://numpy.org/) packages to be able to reproduce the test plots (optional).

In the following, the directory where the top-level `CMakeLists.txt` is located in a downloaded version of `nutr` is denoted as `NUTR_SOURCE_DIR`.
In a build directory `NUTR_BUILD_DIR`, execute:

    $ cmake $NUTR_SOURCE_DIR
    $ cmake --build .

This creates an executable `nutr` in `NUTR_BUILD_DIR`.

### 2.2 Build Variables

After the first build step, several `CMake` build variables will be available for a customization of the build.
Besides the usual Geant4 build variables, `nutr` provides the following options:

* `BUILD_DOCUMENTATION`: Create the code documentation using Doxygen (default: OFF).
* `GEOMETRY_DIR`: Select directory in `$NUTR_SOURCE_DIR/src/geometry` that contains the desired geometry.
* `OUTPUT_FORMAT`: Determine the format of the output. Possible choices: `root` (default), `csv`, and `xml`.
* `PRIMARY_GENERATOR_DIR`: Select directory in `$NUTR_SOURCE_DIR/src/fundamentals/primary_generator` that contains the desired primary generator (default: `general_particle_source`).
* `SENSITIVE_DETECTOR_DIR`: Select directory in `$NUTR_SOURCE_DIR/src/sensitive_detector` that contains the desired sensitive detector (default: `edep`).
* `WITH_GEANT4_UIVIS`: Build `nutr` with Geant4 UI and Vis drivers (default: ON).

## 3. License

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

Copyright (C) 2020 Udo Friman-Gayer (ufg@email.unc.edu)

## 4. Acknowledgements

The author would like to thank C. Iliadis for enlightening discussions about the angular correlation formalism and for help with debugging the associated modules.
The author would also like to thank O. Papst for helpful discussions about the angular correlation formalism and advertise OP's angular correlation code [angcorrwat](https://github.com/op3/angcorrwat).
`angcorrwat` is complementary to the present code in the sense that it uses the python package [sympy](https://www.sympy.org/) to obtain symbolic expressions for the angular correlations.

## 5. References

[1] S. Agostinelli *et al.*, “GEANT4 - a simulation toolkit”, Nucl. Inst. Meth. A **506**, 250-303 (2003). [`doi:10.1016/S0168-9002(03)01368-8`](https://doi.org/10.1016/S0168-9002(03)01368-8); J. Allison *et al.*, “GEANT4 developments and applications”, IEEE T. Nucl. Sci., **53**, 270-278 (2006). [`doi:10.1109/TNS.2006.869826`](https://doi.org/10.1109/TNS.2006.869826); J. Allison *et al.*, “Recent developments in GEANT4”, Nucl. Inst. Meth. A **835**, 186-225 (2016). [`doi:10.1016/j.nima.2016.06.125`](https://doi.org/10.1016/j.nima.2016.06.125)

[2] H. R. Weller *et al.*, “Research opportunities at the upgraded HIγS facility”, Prog. Part. Nucl. Phys. **62**, 257-303 (2009). [`doi:10.1016/j.ppnp.2008.07.001`](https://doi.org/10.1016/j.ppnp.2008.07.001)

[3] U. Friman-Gayer, J. Kleemann, and O. Papst, “GEANT4 simulation of the Upstream Target Room (UTR) at the HIγS facility“ (2019) [`doi:10.5281/zenodo.3430154`](https://doi.org/10.5281/zenodo.3430154)

See also `NUTR_SOURCE_DIR/bibliography.bib`.
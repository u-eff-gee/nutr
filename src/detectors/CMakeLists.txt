#    This file is part of nutr.
#
#    nutr is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    nutr is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with nutr.  If not, see <https://www.gnu.org/licenses/>.
#
#    Copyright (C) 2020, 2021 Udo Friman-Gayer

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include/detectors)

add_library(detector Detector.cc)
target_compile_options(detector PUBLIC -Wall -Wextra)

add_library(filterCase Filter_Case.cc)

add_library(hpgeClover HPGe_Clover.cc)
target_link_libraries(hpgeClover detector)

add_library(hpgeCoaxial HPGe_Coaxial.cc)
target_link_libraries(hpgeCoaxial detector filterCase)

add_library(nai NaI.cc)
target_link_libraries(nai detector)

add_library(scintillatorSCIONIX CeBr3_15x15.cc)
#target_include_directories(scintillatorSCIONIX PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry/materials)
target_link_libraries(scintillatorSCIONIX cebr3 detector)
# This file is part of nutr.
#
# nutr is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# nutr is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# nutr.  If not, see <https://www.gnu.org/licenses/>.
#
# Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst

include_directories(${PROJECT_SOURCE_DIR}/include/sensitive_detector)
include_directories(${PROJECT_BINARY_DIR}/include/sensitive_detector)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

set(UPDATE_FREQUENCY "10000" CACHE STRING "Determine the number of events since the last update after which a new update about the progress of the simulation is printed on the command line (default: 10000).")

set(SENSITIVE_DETECTOR_DIR "event" CACHE STRING "Select directory in `${PROJECT_SOURCE_DIR}/src/sensitive_detector` that contains the desired sensitive detector. Possible choices: `edep`, `event` (default), `flux`, `tracker`.")

option(TRACK_PRIMARY "Track position and momentum of (first) primary vertex per event" Off)

configure_file(
  ${PROJECT_SOURCE_DIR}/include/sensitive_detector/SensitiveDetectorBuildOptions.hh.in
  ${PROJECT_BINARY_DIR}/include/sensitive_detector/SensitiveDetectorBuildOptions.hh
)

add_library(analysisManager AnalysisManager.cc)

add_library(nDetectorHit NDetectorHit.cc)

add_library(nRunAction NRunAction.cc)

add_library(nEventAction NEventAction.cc)
target_link_libraries(nEventAction nRunAction)

add_library(nSensitiveDetector NSensitiveDetector.cc)

add_subdirectory(${SENSITIVE_DETECTOR_DIR})

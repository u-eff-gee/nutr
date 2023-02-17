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
include_directories(${PROJECT_SOURCE_DIR}/include/fundamentals)
include_directories(${PROJECT_BINARY_DIR}/include/sensitive_detector)

set(UPDATE_FREQUENCY
    "10000"
    CACHE
      STRING
      "Determine the number of events since the last update after which a new update about the progress of the simulation is printed on the command line (default: 10000)."
)

option(TRACK_PRIMARY
       "Track position and momentum of (first) primary vertex per event" Off)

configure_file(
  ${PROJECT_SOURCE_DIR}/include/sensitive_detector/SensitiveDetectorBuildOptions.hh.in
  ${PROJECT_BINARY_DIR}/include/sensitive_detector/SensitiveDetectorBuildOptions.hh
)

add_library(analysisManager AnalysisManager.cc)
target_include_directories(analysisManager PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(nDetectorHit NDetectorHit.cc)
target_include_directories(nDetectorHit PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(nRunAction NRunAction.cc)
target_include_directories(nRunAction PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(nEventAction NEventAction.cc)
target_link_libraries(nEventAction nRunAction)

add_library(nSensitiveDetector NSensitiveDetector.cc)
target_include_directories(nSensitiveDetector PUBLIC ${Geant4_INCLUDE_DIRS})

add_subdirectory(${SENSITIVE_DETECTOR_DIR})

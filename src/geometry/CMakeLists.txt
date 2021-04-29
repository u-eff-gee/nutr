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

add_library(nDetectorConstruction NDetectorConstruction.cc)
target_include_directories(nDetectorConstruction PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(nDetectorConstruction SensitiveDetector)

add_library(sourceVolume EXCLUDE_FROM_ALL SourceVolume.cc)
target_include_directories(sourceVolume PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry)

add_library(sourceVolumeTubs EXCLUDE_FROM_ALL SourceVolumeTubs.cc)
target_include_directories(sourceVolumeTubs PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry)
target_link_libraries(sourceVolumeTubs sourceVolume)
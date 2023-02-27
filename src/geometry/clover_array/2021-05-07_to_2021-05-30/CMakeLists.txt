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
#    Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst

add_library(detectorConstructionMessenger_2021-05-07_to_2021-05-30 DetectorConstructionMessenger.cc)
target_include_directories(detectorConstructionMessenger_2021-05-07_to_2021-05-30 PUBLIC  ${Geant4_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2021-05-07_to_2021-05-30)

add_library(detectorConstruction_2021-05-07_to_2021-05-30 DetectorConstruction.cc)
target_include_directories(detectorConstruction_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/detectors ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2021-05-07_to_2021-05-30 ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(detectorConstruction_2021-05-07_to_2021-05-30 activation_target beamPipe cebr3_2x2 collimatorRoom detectorConstructionMessenger_2021-05-07_to_2021-05-30 mechanical nDetectorConstruction gamma_vault hpgeCoaxial hpgeClover labr3ce_3x3 leadShieldingUTR_2021-05-07_to_2021-05-31 molly SensitiveDetector zero_degree_mechanical)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2021-05-07_to_2021-05-30)
add_executable(gps_2021-05-07_to_2021-05-30 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(gps_2021-05-07_to_2021-05-30 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2021-05-07_to_2021-05-30)
target_include_directories(gps_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(gps_2021-05-07_to_2021-05-30 actionInitialization_2021-05-07_to_2021-05-30 ${Boost_LIBRARIES} detectorConstruction_2021-05-07_to_2021-05-30 physics ${Geant4_LIBRARIES})

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2021-05-07_to_2021-05-30)
add_executable(angcorr_2021-05-07_to_2021-05-30 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(angcorr_2021-05-07_to_2021-05-30 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2021-05-07_to_2021-05-30)
target_include_directories(angcorr_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(angcorr_2021-05-07_to_2021-05-30 actionInitialization_2021-05-07_to_2021-05-30_angcorr ${Boost_LIBRARIES} detectorConstruction_2021-05-07_to_2021-05-30 physics ${Geant4_LIBRARIES})

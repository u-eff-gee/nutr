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

add_library(detectorConstruction_2022-02-07_to_2022-02-15 DetectorConstruction.cc)
target_include_directories(detectorConstruction_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/detectors ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2022-02-07_to_2022-02-15 ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(detectorConstruction_2022-02-07_to_2022-02-15 beamPipe cebr3_2x2 collimatorRoom gamma_vault mechanical nDetectorConstruction hpgeCoaxial hpgeClover labr3ce_3x3 leadShieldingUTR_2022-01-21_to_2022-03-07 molly SensitiveDetector zero_degree_mechanical)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2022-02-07_to_2022-02-15)
add_executable(gps_2022-02-07_to_2022-02-15 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(gps_2022-02-07_to_2022-02-15 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2022-02-07_to_2022-02-15)
target_include_directories(gps_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(gps_2022-02-07_to_2022-02-15 actionInitialization_2022-02-07_to_2022-02-15 ${Boost_LIBRARIES} detectorConstruction_2022-02-07_to_2022-02-15 physics ${Geant4_LIBRARIES})

add_executable(angcorr_2022-02-07_to_2022-02-15 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(angcorr_2022-02-07_to_2022-02-15 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2022-02-07_to_2022-02-15)
target_include_directories(angcorr_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(angcorr_2022-02-07_to_2022-02-15 actionInitialization_2022-02-07_to_2022-02-15_angcorr ${Boost_LIBRARIES} detectorConstruction_2022-02-07_to_2022-02-15 physics ${Geant4_LIBRARIES})

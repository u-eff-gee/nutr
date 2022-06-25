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

add_library(detectorConstruction_2021-08-24_to_2021-08-25 DetectorConstruction.cc)
target_include_directories(detectorConstruction_2021-08-24_to_2021-08-25 PUBLIC ${PROJECT_SOURCE_DIR}/include/detectors ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2021-08-24_to_2021-08-25 ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(detectorConstruction_2021-08-24_to_2021-08-25 beamPipe cebr3_2x2 collimatorRoom gamma_vault mechanical nDetectorConstruction hpgeCoaxial hpgeClover labr3ce_3x3 leadShieldingUTR_2021-08-23_to_2021-09-09 molly SensitiveDetector zero_degree_mechanical)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2021-08-24_to_2021-08-25)
add_executable(nutr_2021-08-24_to_2021-08-25 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(nutr_2021-08-24_to_2021-08-25 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2021-08-24_to_2021-08-25)
target_include_directories(nutr_2021-08-24_to_2021-08-25 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(nutr_2021-08-24_to_2021-08-25 actionInitialization_2021-08-24_to_2021-08-25 ${Boost_LIBRARIES} detectorConstruction_2021-08-24_to_2021-08-25 physics ${Geant4_LIBRARIES})
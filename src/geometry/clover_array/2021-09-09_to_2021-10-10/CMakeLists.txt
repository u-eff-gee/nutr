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

set(COAXIAL_B4 "HPGe_ANL_41203" CACHE STRING "Detector at position B4")
set_property(CACHE
    COAXIAL_B4
    PROPERTY
    STRINGS
    "HPGe_ANL_41203"
    "HPGe_ANL_31670")
set(COAXIAL_B4_DEWAR "${COAXIAL_B4}_Dewar" CACHE STRING "Dewar of detector B4")
configure_file(
	"${CMAKE_CURRENT_LIST_DIR}/CoaxB4Config.hh.in"
	"${PROJECT_BINARY_DIR}/src/geometry/clover_array/2021-09-09_to_2021-10-10/CoaxB4Config.hh"
)

add_library(detectorConstruction_2021-09-09_to_2021-10-10 DetectorConstruction.cc)
target_include_directories(detectorConstruction_2021-09-09_to_2021-10-10
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/detectors
    ${PROJECT_SOURCE_DIR}/include/geometry
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2021-09-09_to_2021-10-10
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array
    ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR}
    ${PROJECT_BINARY_DIR}/src/geometry/clover_array/2021-09-09_to_2021-10-10
)
target_link_libraries(detectorConstruction_2021-09-09_to_2021-10-10
    beamPipe
    collimatorRoom
    target96Mo
    comptonMonitor_2021-09-09_to_2021-10-10
    mechanical
    nDetectorConstruction
    hpgeClover
    hpgeCoaxial
    labr3ce_3x3
    leadShieldingUTR_2021-09-10_to_2021-10-10
    SensitiveDetector
    cebr3_2x2
    zero_degree_mechanical
)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2021-09-09_to_2021-10-10)
add_executable(nutr_2021-09-09_to_2021-10-10 ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(nutr_2021-09-09_to_2021-10-10 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/2021-09-09_to_2021-10-10)
target_include_directories(nutr_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(nutr_2021-09-09_to_2021-10-10 actionInitialization_2021-09-09_to_2021-10-10 ${Boost_LIBRARIES} detectorConstruction_2021-09-09_to_2021-10-10 physics ${Geant4_LIBRARIES})
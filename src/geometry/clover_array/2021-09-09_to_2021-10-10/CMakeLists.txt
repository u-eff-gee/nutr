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
configure_file(
	"${CMAKE_CURRENT_LIST_DIR}/CoaxB4Config.hh.in"
	"${PROJECT_BINARY_DIR}/src/geometry/clover_array/2021-09-09_to_2021-10-10/CoaxB4Config.hh"
)

add_library(detectorConstruction DetectorConstruction.cc)
target_include_directories(detectorConstruction
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/detectors
    ${PROJECT_SOURCE_DIR}/include/geometry
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2021-09-09_to_2021-10-10
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array
    ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR}
    ${PROJECT_BINARY_DIR}/src/geometry/clover_array/2021-09-09_to_2021-10-10
)
target_link_libraries(detectorConstruction
    beamPipe
    collimatorRoom
    target96Mo
    comptonMonitor_2021-09-09_to_2021-10-10
    mechanical
    nDetectorConstruction
    hpgeClover
    hpgeCoaxial
    labr3ce_3x3
    leadShieldingUTR
    SensitiveDetector
    cebr3_2x2
)

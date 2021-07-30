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

add_library(detectorConstruction DetectorConstruction.cc)
target_include_directories(detectorConstruction PUBLIC ${PROJECT_SOURCE_DIR}/include/detectors ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/02_16_2021_to_04_10_2021 ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(detectorConstruction beamPipe collimatorRoom comptonMonitor_02_16_2021_to_04_18_2021 nDetectorConstruction hpgeClover labr3ce_3x3 leadShieldingUTR SensitiveDetector cebr3_15x15 sourceVolumeTubs)
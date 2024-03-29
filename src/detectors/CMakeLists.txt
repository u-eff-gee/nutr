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

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include/detectors)

add_library(detector_channel_messenger DetectorChannelMessenger.cc)

add_library(detector Detector.cc)
target_link_libraries(detector detector_channel_messenger)

add_library(hpgeClover EXCLUDE_FROM_ALL HPGe_Clover.cc)
target_link_libraries(hpgeClover detector pla)

add_library(hpgeCoaxial EXCLUDE_FROM_ALL HPGe_Coaxial.cc)
target_link_libraries(hpgeCoaxial detector pla)

add_library(cebr3_2x2 EXCLUDE_FROM_ALL CeBr3_2x2.cc)
target_link_libraries(cebr3_2x2 cebr3 detector)

add_library(labr3ce_3x3 EXCLUDE_FROM_ALL LaBr3Ce_3x3.cc)
target_link_libraries(labr3ce_3x3 labr3ce detector pla)

add_library(molly EXCLUDE_FROM_ALL MOLLY.cc)
target_link_libraries(molly detector)
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

include_directories(${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

add_library(primaryGeneratorActionAngCorr PrimaryGeneratorAction.cc
                                          PrimaryGeneratorMessenger.cc)
target_include_directories(
  primaryGeneratorActionAngCorr
  PUBLIC ${PROJECT_SOURCE_DIR}/include/angular_correlation
         ${PROJECT_SOURCE_DIR}/include/geometry/)
target_link_libraries(primaryGeneratorActionAngCorr angular_correlation
                      cascadeRejectionSampler sourceVolume)

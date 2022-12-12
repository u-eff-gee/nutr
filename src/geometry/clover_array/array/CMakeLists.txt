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

include_directories(${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array)

add_library(activation_target EXCLUDE_FROM_ALL ActivationTarget.cc)
target_include_directories(activation_target PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(beamPipe EXCLUDE_FROM_ALL BeamPipe.cc)
target_include_directories(beamPipe PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(collimatorRoom EXCLUDE_FROM_ALL CollimatorRoom.cc)
target_include_directories(collimatorRoom PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(comptonMonitorTarget EXCLUDE_FROM_ALL ComptonMonitorTarget.cc)
target_include_directories(comptonMonitorTarget PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(target96Mo EXCLUDE_FROM_ALL Target96Mo.cc)
target_include_directories(target96Mo PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry)
target_link_libraries(target96Mo PUBLIC sourceVolume sourceVolumeTubs)

add_library(comptonMonitor_2021-02-16_to_2021-04-18 EXCLUDE_FROM_ALL ComptonMonitor_2021-02-16_to_2021-04-18.cc)
target_link_libraries(comptonMonitor_2021-02-16_to_2021-04-18 comptonMonitorTarget)

add_library(comptonMonitor_2021-04-19_to_2021-04-30 EXCLUDE_FROM_ALL ComptonMonitor_2021-04-19_to_2021-04-30.cc)
target_link_libraries(comptonMonitor_2021-04-19_to_2021-04-30 comptonMonitorTarget)

add_library(comptonMonitor_2021-09-09_to_2021-10-10 EXCLUDE_FROM_ALL ComptonMonitor_2021-09-09_to_2021-10-10.cc)
target_link_libraries(comptonMonitor_2021-09-09_to_2021-10-10 comptonMonitorTarget)

add_library(gamma_vault EXCLUDE_FROM_ALL GammaVault.cc)
target_include_directories(gamma_vault PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2021-02-16_to_2021-05-06 EXCLUDE_FROM_ALL LeadShieldingUTR_2021-02-16_to_2021-05-06.cc)
target_include_directories(leadShieldingUTR_2021-02-16_to_2021-05-06 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2021-05-07_to_2021-05-31 EXCLUDE_FROM_ALL LeadShieldingUTR_2021-05-07_to_2021-05-31.cc)
target_include_directories(leadShieldingUTR_2021-05-07_to_2021-05-31 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2021-08-23_to_2021-09-09 EXCLUDE_FROM_ALL LeadShieldingUTR_2021-08-23_to_2021-09-09.cc)
target_include_directories(leadShieldingUTR_2021-08-23_to_2021-09-09 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2021-09-10_to_2021-10-10 EXCLUDE_FROM_ALL LeadShieldingUTR_2021-09-10_to_2021-10-10.cc)
target_include_directories(leadShieldingUTR_2021-09-10_to_2021-10-10 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2021-11-08_to_2021-11-21 EXCLUDE_FROM_ALL LeadShieldingUTR_2021-11-08_to_2021-11-21.cc)
target_include_directories(leadShieldingUTR_2021-11-08_to_2021-11-21 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(leadShieldingUTR_2022-01-21_to_2022-03-07 EXCLUDE_FROM_ALL LeadShieldingUTR_2022-01-21_to_2022-03-07.cc)
target_include_directories(leadShieldingUTR_2022-01-21_to_2022-03-07 PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(mechanical EXCLUDE_FROM_ALL Mechanical.cc)
target_include_directories(mechanical PUBLIC ${Geant4_INCLUDE_DIRS})

add_library(zero_degree_mechanical EXCLUDE_FROM_ALL ZeroDegreeMechanical.cc)
target_include_directories(zero_degree_mechanical PUBLIC ${Geant4_INCLUDE_DIRS})

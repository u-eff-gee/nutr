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
#    Copyright (C) 2020-2022 Udo Friman-Gayer

include_directories(${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array)

add_library(beamPipe EXCLUDE_FROM_ALL BeamPipe.cc)

add_library(collimatorRoom EXCLUDE_FROM_ALL CollimatorRoom.cc)

add_library(comptonMonitorTarget EXCLUDE_FROM_ALL ComptonMonitorTarget.cc)

add_library(target96Mo EXCLUDE_FROM_ALL Target96Mo.cc)

add_library(comptonMonitor_02_16_2021_to_04_18_2021 EXCLUDE_FROM_ALL ComptonMonitor_02_16_2021_to_04_18_2021.cc)
target_link_libraries(comptonMonitor_02_16_2021_to_04_18_2021 comptonMonitorTarget)

add_library(comptonMonitor_04_19_2021_to_04_30_2021 EXCLUDE_FROM_ALL ComptonMonitor_04_19_2021_to_04_30_2021.cc)
target_link_libraries(comptonMonitor_04_19_2021_to_04_30_2021 comptonMonitorTarget)

add_library(comptonMonitor_2021-09-09_to_2021-10-10 EXCLUDE_FROM_ALL ComptonMonitor_2021-09-09_to_2021-10-10.cc)
target_link_libraries(comptonMonitor_2021-09-09_to_2021-10-10 comptonMonitorTarget)

add_library(leadShieldingUTR EXCLUDE_FROM_ALL LeadShieldingUTR.cc)

add_library(mechanical EXCLUDE_FROM_ALL Mechanical.cc)
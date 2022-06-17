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

include_directories(${PROJECT_SOURCE_DIR}/include/sensitive_detector/event)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

add_library(DetectorHit DetectorHit.cc)
target_include_directories(DetectorHit PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector)
target_link_libraries(DetectorHit nDetectorHit)

add_library(SensitiveDetector SensitiveDetector.cc)
target_link_libraries(SensitiveDetector nSensitiveDetector DetectorHit)

if(${GEOMETRY_2021-02-16_to_2021-04-10})
    add_library(tupleManager_2021-02-16_to_2021-04-10 TupleManager.cc)
    target_include_directories(tupleManager_2021-02-16_to_2021-04-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-02-16_to_2021-04-10 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-02-16_to_2021-04-10 analysisManager detectorConstruction_2021-02-16_to_2021-04-10 DetectorHit)

    add_library(eventAction_2021-02-16_to_2021-04-10 EventAction.cc)
    target_include_directories(eventAction_2021-02-16_to_2021-04-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-02-16_to_2021-04-10 nEventAction tupleManager_2021-02-16_to_2021-04-10)
endif()

if(${GEOMETRY_2021-04-19_to_2021-04-30})
    add_library(tupleManager_2021-04-19_to_2021-04-30 TupleManager.cc)
    target_include_directories(tupleManager_2021-04-19_to_2021-04-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-04-19_to_2021-04-30 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-04-19_to_2021-04-30 analysisManager detectorConstruction_2021-04-19_to_2021-04-30 DetectorHit)

    add_library(eventAction_2021-04-19_to_2021-04-30 EventAction.cc)
    target_include_directories(eventAction_2021-04-19_to_2021-04-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-04-19_to_2021-04-30 nEventAction tupleManager_2021-04-19_to_2021-04-30)
endif()

if(${GEOMETRY_2021-05-07_to_2021-05-30})
    add_library(tupleManager_2021-05-07_to_2021-05-30 TupleManager.cc)
    target_include_directories(tupleManager_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-05-07_to_2021-05-30 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-05-07_to_2021-05-30 analysisManager detectorConstruction_2021-05-07_to_2021-05-30 DetectorHit)

    add_library(eventAction_2021-05-07_to_2021-05-30 EventAction.cc)
    target_include_directories(eventAction_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-05-07_to_2021-05-30 nEventAction tupleManager_2021-05-07_to_2021-05-30)
endif()

if(${GEOMETRY_2021-09-09_to_2021-10-10})
    add_library(tupleManager_2021-09-09_to_2021-10-10 TupleManager.cc)
    target_include_directories(tupleManager_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-09-09_to_2021-10-10 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-09-09_to_2021-10-10 analysisManager detectorConstruction_2021-09-09_to_2021-10-10 DetectorHit)

    add_library(eventAction_2021-09-09_to_2021-10-10 EventAction.cc)
    target_include_directories(eventAction_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-09-09_to_2021-10-10 nEventAction tupleManager_2021-09-09_to_2021-10-10)
endif()
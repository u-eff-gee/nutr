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

include_directories(${PROJECT_SOURCE_DIR}/include/sensitive_detector/edep)

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

if(${GEOMETRY_2021-08-23})
    add_library(tupleManager_2021-08-23 TupleManager.cc)
    target_include_directories(tupleManager_2021-08-23 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-08-23 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-08-23 analysisManager detectorConstruction_2021-08-23 DetectorHit)

    add_library(eventAction_2021-08-23 EventAction.cc)
    target_include_directories(eventAction_2021-08-23 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-08-23 nEventAction tupleManager_2021-08-23)
endif()

if(${GEOMETRY_2021-08-24_to_2021-08-25})
    add_library(tupleManager_2021-08-24_to_2021-08-25 TupleManager.cc)
    target_include_directories(tupleManager_2021-08-24_to_2021-08-25 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-08-24_to_2021-08-25 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-08-24_to_2021-08-25 analysisManager detectorConstruction_2021-08-24_to_2021-08-25 DetectorHit)

    add_library(eventAction_2021-08-24_to_2021-08-25 EventAction.cc)
    target_include_directories(eventAction_2021-08-24_to_2021-08-25 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-08-24_to_2021-08-25 nEventAction tupleManager_2021-08-24_to_2021-08-25)
endif()

if(${GEOMETRY_2021-08-25_to_2021-08-27})
    add_library(tupleManager_2021-08-25_to_2021-08-27 TupleManager.cc)
    target_include_directories(tupleManager_2021-08-25_to_2021-08-27 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-08-25_to_2021-08-27 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-08-25_to_2021-08-27 analysisManager detectorConstruction_2021-08-25_to_2021-08-27 DetectorHit)

    add_library(eventAction_2021-08-25_to_2021-08-27 EventAction.cc)
    target_include_directories(eventAction_2021-08-25_to_2021-08-27 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-08-25_to_2021-08-27 nEventAction tupleManager_2021-08-25_to_2021-08-27)
endif()

if(${GEOMETRY_2021-08-28_to_2021-09-09})
    add_library(tupleManager_2021-08-28_to_2021-09-09 TupleManager.cc)
    target_include_directories(tupleManager_2021-08-28_to_2021-09-09 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-08-28_to_2021-09-09 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-08-28_to_2021-09-09 analysisManager detectorConstruction_2021-08-28_to_2021-09-09 DetectorHit)

    add_library(eventAction_2021-08-28_to_2021-09-09 EventAction.cc)
    target_include_directories(eventAction_2021-08-28_to_2021-09-09 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-08-28_to_2021-09-09 nEventAction tupleManager_2021-08-28_to_2021-09-09)
endif()

if(${GEOMETRY_2021-09-09_to_2021-10-10})
    add_library(tupleManager_2021-09-09_to_2021-10-10 TupleManager.cc)
    target_include_directories(tupleManager_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-09-09_to_2021-10-10 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-09-09_to_2021-10-10 analysisManager detectorConstruction_2021-09-09_to_2021-10-10 DetectorHit)

    add_library(eventAction_2021-09-09_to_2021-10-10 EventAction.cc)
    target_include_directories(eventAction_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-09-09_to_2021-10-10 nEventAction tupleManager_2021-09-09_to_2021-10-10)
endif()

if(${GEOMETRY_2021-11-08_to_2021-11-21})
    add_library(tupleManager_2021-11-08_to_2021-11-21 TupleManager.cc)
    target_include_directories(tupleManager_2021-11-08_to_2021-11-21 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2021-11-08_to_2021-11-21 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2021-11-08_to_2021-11-21 analysisManager detectorConstruction_2021-11-08_to_2021-11-21 DetectorHit)

    add_library(eventAction_2021-11-08_to_2021-11-21 EventAction.cc)
    target_include_directories(eventAction_2021-11-08_to_2021-11-21 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2021-11-08_to_2021-11-21 nEventAction tupleManager_2021-11-08_to_2021-11-21)
endif()

if(${GEOMETRY_2022-01-21_to_2022-02-05})
    add_library(tupleManager_2022-01-21_to_2022-02-05 TupleManager.cc)
    target_include_directories(tupleManager_2022-01-21_to_2022-02-05 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2022-01-21_to_2022-02-05 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2022-01-21_to_2022-02-05 analysisManager detectorConstruction_2022-01-21_to_2022-02-05 DetectorHit)

    add_library(eventAction_2022-01-21_to_2022-02-05 EventAction.cc)
    target_include_directories(eventAction_2022-01-21_to_2022-02-05 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2022-01-21_to_2022-02-05 nEventAction tupleManager_2022-01-21_to_2022-02-05)
endif()

if(${GEOMETRY_2022-02-07_to_2022-02-15})
    add_library(tupleManager_2022-02-07_to_2022-02-15 TupleManager.cc)
    target_include_directories(tupleManager_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2022-02-07_to_2022-02-15 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2022-02-07_to_2022-02-15 analysisManager detectorConstruction_2022-02-07_to_2022-02-15 DetectorHit)

    add_library(eventAction_2022-02-07_to_2022-02-15 EventAction.cc)
    target_include_directories(eventAction_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2022-02-07_to_2022-02-15 nEventAction tupleManager_2022-02-07_to_2022-02-15)
endif()

if(${GEOMETRY_2022-02-21_to_2022-03-02})
    add_library(tupleManager_2022-02-21_to_2022-03-02 TupleManager.cc)
    target_include_directories(tupleManager_2022-02-21_to_2022-03-02 PUBLIC ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/geometry/2022-02-21_to_2022-03-02 ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(tupleManager_2022-02-21_to_2022-03-02 analysisManager detectorConstruction_2022-02-21_to_2022-03-02 DetectorHit)

    add_library(eventAction_2022-02-21_to_2022-03-02 EventAction.cc)
    target_include_directories(eventAction_2022-02-21_to_2022-03-02 PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector ${PROJECT_BINARY_DIR}/include/sensitive_detector)
    target_link_libraries(eventAction_2022-02-21_to_2022-03-02 nEventAction tupleManager_2022-02-21_to_2022-03-02)
endif()
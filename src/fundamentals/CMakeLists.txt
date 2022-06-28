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

include_directories(${PROJECT_SOURCE_DIR}/include/fundamentals)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

if(${GEOMETRY_2021-02-16_to_2021-04-10})
    add_library(actionInitialization_2021-02-16_to_2021-04-10 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-02-16_to_2021-04-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-02-16_to_2021-04-10 eventAction_2021-02-16_to_2021-04-10 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-02-16_to_2021-04-10_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-02-16_to_2021-04-10_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-02-16_to_2021-04-10_angcorr eventAction_2021-02-16_to_2021-04-10 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-04-19_to_2021-04-30})
    add_library(actionInitialization_2021-04-19_to_2021-04-30 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-04-19_to_2021-04-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-04-19_to_2021-04-30 eventAction_2021-04-19_to_2021-04-30 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-04-19_to_2021-04-30_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-04-19_to_2021-04-30_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-04-19_to_2021-04-30_angcorr eventAction_2021-04-19_to_2021-04-30 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-05-07_to_2021-05-30})
    add_library(actionInitialization_2021-05-07_to_2021-05-30 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-05-07_to_2021-05-30 eventAction_2021-05-07_to_2021-05-30 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-05-07_to_2021-05-30_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-05-07_to_2021-05-30_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-05-07_to_2021-05-30_angcorr eventAction_2021-05-07_to_2021-05-30 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-08-23})
    add_library(actionInitialization_2021-08-23 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-08-23 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-08-23 eventAction_2021-08-23 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-08-23_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-08-23_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-08-23_angcorr eventAction_2021-08-23 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-08-24_to_2021-08-25})
    add_library(actionInitialization_2021-08-24_to_2021-08-25 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-08-24_to_2021-08-25 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-08-24_to_2021-08-25 eventAction_2021-08-24_to_2021-08-25 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-08-24_to_2021-08-25_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-08-24_to_2021-08-25_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-08-24_to_2021-08-25_angcorr eventAction_2021-08-24_to_2021-08-25 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-08-25_to_2021-08-27})
    add_library(actionInitialization_2021-08-25_to_2021-08-27 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-08-25_to_2021-08-27 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-08-25_to_2021-08-27 eventAction_2021-08-25_to_2021-08-27 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-08-25_to_2021-08-27_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-08-25_to_2021-08-27_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-08-25_to_2021-08-27_angcorr eventAction_2021-08-25_to_2021-08-27 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-08-28_to_2021-09-09})
    add_library(actionInitialization_2021-08-28_to_2021-09-09 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-08-28_to_2021-09-09 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-08-28_to_2021-09-09 eventAction_2021-08-28_to_2021-09-09 primaryGeneratorAction nRunAction)

    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-08-28_to_2021-09-09_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-08-28_to_2021-09-09_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-08-28_to_2021-09-09_angcorr eventAction_2021-08-28_to_2021-09-09 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-09-09_to_2021-10-10})
    add_library(actionInitialization_2021-09-09_to_2021-10-10 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-09-09_to_2021-10-10 eventAction_2021-09-09_to_2021-10-10 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-09-09_to_2021-10-10_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-09-09_to_2021-10-10_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-09-09_to_2021-10-10_angcorr eventAction_2021-09-09_to_2021-10-10 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2021-11-08_to_2021-11-21})
    add_library(actionInitialization_2021-11-08_to_2021-11-21 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-11-08_to_2021-11-21 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-11-08_to_2021-11-21 eventAction_2021-11-08_to_2021-11-21 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2021-11-08_to_2021-11-21_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2021-11-08_to_2021-11-21_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2021-11-08_to_2021-11-21_angcorr eventAction_2021-11-08_to_2021-11-21 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2022-01-21_to_2022-02-05})
    add_library(actionInitialization_2022-01-21_to_2022-02-05 ActionInitialization.cc)
    target_include_directories(actionInitialization_2022-01-21_to_2022-02-05 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2022-01-21_to_2022-02-05 eventAction_2022-01-21_to_2022-02-05 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2022-01-21_to_2022-02-05_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2022-01-21_to_2022-02-05_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2022-01-21_to_2022-02-05_angcorr eventAction_2022-01-21_to_2022-02-05 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2022-02-07_to_2022-02-15})
    add_library(actionInitialization_2022-02-07_to_2022-02-15 ActionInitialization.cc)
    target_include_directories(actionInitialization_2022-02-07_to_2022-02-15 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2022-02-07_to_2022-02-15 eventAction_2022-02-07_to_2022-02-15 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2022-02-07_to_2022-02-15_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2022-02-07_to_2022-02-15_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2022-02-07_to_2022-02-15_angcorr eventAction_2022-02-07_to_2022-02-15 primaryGeneratorActionAngCorr nRunAction)
    endif()
endif()

if(${GEOMETRY_2022-02-21_to_2022-03-02})
    add_library(actionInitialization_2022-02-21_to_2022-03-02 ActionInitialization.cc)
    target_include_directories(actionInitialization_2022-02-21_to_2022-03-02 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2022-02-21_to_2022-03-02 eventAction_2022-02-21_to_2022-03-02 primaryGeneratorAction nRunAction)
    if(${alpaca_FOUND})
        add_library(actionInitialization_2022-02-21_to_2022-03-02_angcorr ActionInitialization.cc)
        target_include_directories(actionInitialization_2022-02-21_to_2022-03-02_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
        target_link_libraries(actionInitialization_2022-02-21_to_2022-03-02_angcorr eventAction_2022-02-21_to_2022-03-02 primaryGeneratorAction nRunAction)
    endif()
endif()
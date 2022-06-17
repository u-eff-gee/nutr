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
    target_include_directories(actionInitialization_2021-02-16_to_2021-04-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/${PRIMARY_GENERATOR_DIR} ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-02-16_to_2021-04-10 eventAction_2021-02-16_to_2021-04-10 primaryGeneratorAction nRunAction)
endif()

if(${GEOMETRY_2021-04-19_to_2021-04-30})
    add_library(actionInitialization_2021-04-19_to_2021-04-30 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-04-19_to_2021-04-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/${PRIMARY_GENERATOR_DIR} ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-04-19_to_2021-04-30 eventAction_2021-04-19_to_2021-04-30 primaryGeneratorAction nRunAction)
endif()

if(${GEOMETRY_2021-05-07_to_2021-05-30})
    add_library(actionInitialization_2021-05-07_to_2021-05-30 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-05-07_to_2021-05-30 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/${PRIMARY_GENERATOR_DIR} ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-05-07_to_2021-05-30 eventAction_2021-05-07_to_2021-05-30 primaryGeneratorAction nRunAction)
endif()

if(${GEOMETRY_2021-09-09_to_2021-10-10})
    add_library(actionInitialization_2021-09-09_to_2021-10-10 ActionInitialization.cc)
    target_include_directories(actionInitialization_2021-09-09_to_2021-10-10 PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/${PRIMARY_GENERATOR_DIR} ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
    target_link_libraries(actionInitialization_2021-09-09_to_2021-10-10 eventAction_2021-09-09_to_2021-10-10 primaryGeneratorAction nRunAction)
endif()
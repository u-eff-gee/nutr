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
#    Copyright (C) 2020 Udo Friman-Gayer

include_directories(${PROJECT_SOURCE_DIR}/include/fundamentals)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})

add_subdirectory(${PROJECT_SOURCE_DIR}/src/fundamentals/analysis)
add_library(eventAction EventAction.cc)
target_include_directories(eventAction PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals/analysis)
target_link_libraries(eventAction tupleManager)

add_library(runAction RunAction.cc)
target_include_directories(runAction PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals/analysis)
target_link_libraries(runAction tupleManager)

set(PRIMARY_GENERATOR_DIR "general_particle_source" CACHE STRING "Select directory in ${PROJECT_SOURCE_DIR}/src/fundamentals/primary_generator that contains the desired primary generator.")
add_subdirectory(${PROJECT_SOURCE_DIR}/src/fundamentals/primary_generator/${PRIMARY_GENERATOR_DIR})

#if(${PRIMARY_GENERATOR_DIR} STREQUAL "angular_distribution_source")
#    include_directories(${PROJECT_SOURCE_DIR}/include/fundamentals/#primary_generator/angular_distribution_source/angular_distribution)
#endif()

add_library(actionInitialization ActionInitialization.cc)
target_include_directories(actionInitialization PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals/primary_generator/${PRIMARY_GENERATOR_DIR})
target_link_libraries(actionInitialization eventAction primaryGeneratorAction runAction)
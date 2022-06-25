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

# Based on the official CMake tutorial on:
#
# https://cmake.org/cmake/help/latest/guide/tutorial/
#
# In particular, this CMakeLists.txt file and the ones in the subdirectories of src/
# avoid the use of 'globbing', i.e. commands like
#
# file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.cc)
#
# which are often used in Geant4 examples, but discouraged
# (https://cmake.org/cmake/help/latest/command/file.html?highlight=file#filesystem).
#
# Discussions about the use of globbing can be found in various user forums on the web.
# Here, it was decided to follow the CMake tutorial, which means having verbose CMakeLists.txt,
# files, but also a more transparent overview of all dependencies.
# In addition, compiling dependencies as separate libraries facilitates unit testing.

cmake_minimum_required(VERSION 3.7.2)
project(nutr VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

option(WITH_GEANT4_UIVIS "Build nutr with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

option(BUILD_DOCUMENTATION "Build doxygen documentation for some classes of nutr" OFF)
if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  configure_file(${PROJECT_SOURCE_DIR}/Doxyfile.in Doxyfile)
  add_custom_target(doc_doxygen ALL COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)
endif(BUILD_DOCUMENTATION)

option(GEOMETRY_2021-02-16_to_2021-04-10 "Build Clover Array geometry for time interval 2021-02-16 to 2021-04-10." ON)
option(GEOMETRY_2021-04-19_to_2021-04-30 "Build Clover Array geometry for time interval 2021-04-19 to 2021-04-30." ON)
option(GEOMETRY_2021-05-07_to_2021-05-30 "Build Clover Array geometry for time interval 2021-05-07 to 2021-05-30." ON)
option(GEOMETRY_2021-08-23 "Build Clover Array geometry on 2021-08-23." ON)
option(GEOMETRY_2021-08-24_to_2021-08-25 "Build Clover Array geometry for time interval 2021-08-24 to 2021-08-25." ON)
option(GEOMETRY_2021-08-25_to_2021-08-27 "Build Clover Array geometry for time interval 2021-08-25 to 2021-08-27." ON)
option(GEOMETRY_2021-08-28_to_2021-09-09 "Build Clover Array geometry for time interval 2021-08-28 to 2021-09-09." ON)
option(GEOMETRY_2021-09-09_to_2021-10-10 "Build Clover Array geometry for time interval 2021-09-09 to 2021-10-10." ON)

set(PRIMARY_GENERATOR_DIR "gps" CACHE STRING "Select directory in `${PROJECT_SOURCE_DIR}/src/fundamentals/primary_generator` that contains the desired primary generator Possible choices: 'gps' (default), 'angcorr'.")

set(PRODUCTION_CUT_LOW_KEV "0.99" CACHE STRING "Set the lower energy limit of the production cut for gammas, electrons/positrons and protons in keV (default: '0.99', i.e. use default production cut of G4EmLivermorePolarizedPhysics). A straightforward way to view the current production cuts is the '/run/particle/dumpCutValues' macro command.")

set(SENSITIVE_DETECTOR_DIR "event" CACHE STRING "Select directory in `${PROJECT_SOURCE_DIR}/src/sensitive_detector` that contains the desired sensitive detector. Possible choices: `edep`, `event` (default), `flux`, `tracker`.")

set(UPDATE_FREQUENCY "10000" CACHE STRING "Determine the number of events since the last update after which a new update about the progress of the simulation is printed on the command line (default: 10000).")

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(src/detectors)
add_subdirectory(src/fundamentals)
add_subdirectory(src/geometry)
add_subdirectory(src/geometry/materials)
add_subdirectory(src/geometry/clover_array/array)
if(${GEOMETRY_2021-02-16_to_2021-04-10})
  add_subdirectory(src/geometry/clover_array/2021-02-16_to_2021-04-10)
endif()
if(${GEOMETRY_2021-04-19_to_2021-04-30})
  add_subdirectory(src/geometry/clover_array/2021-04-19_to_2021-04-30)
endif()
if(${GEOMETRY_2021-05-07_to_2021-05-30})
  add_subdirectory(src/geometry/clover_array/2021-05-07_to_2021-05-30)
endif()
if(${GEOMETRY_2021-08-23})
  add_subdirectory(src/geometry/clover_array/2021-08-23)
endif()
if(${GEOMETRY_2021-08-24_to_2021-08-25})
  add_subdirectory(src/geometry/clover_array/2021-08-24_to_2021-08-25)
endif()
if(${GEOMETRY_2021-08-25_to_2021-08-27})
  add_subdirectory(src/geometry/clover_array/2021-08-25_to_2021-08-27)
endif()
if(${GEOMETRY_2021-08-28_to_2021-09-09})
  add_subdirectory(src/geometry/clover_array/2021-08-28_to_2021-09-09)
endif()
if(${GEOMETRY_2021-09-09_to_2021-10-10})
  add_subdirectory(src/geometry/clover_array/2021-09-09_to_2021-10-10)
endif()
add_subdirectory(src/physics)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/primary_generator/${PRIMARY_GENERATOR_DIR})
add_subdirectory(src/sensitive_detector)
configure_file(${PROJECT_SOURCE_DIR}/include/sensitive_detector/NEventAction.hh.in ${PROJECT_BINARY_DIR}/include/sensitive_detector/NEventAction.hh)
add_subdirectory(src/sensitive_detector/${SENSITIVE_DETECTOR_DIR})

set(MACROS_ESSENTIAL init_vis.mac vis.mac)
foreach(macro ${MACROS_ESSENTIAL})
  configure_file(${PROJECT_SOURCE_DIR}/macros/${macro} ${PROJECT_BINARY_DIR}/${macro} COPYONLY)
endforeach()

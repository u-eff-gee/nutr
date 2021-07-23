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
#    Copyright (C) 2020, 2021 Udo Friman-Gayer

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
project(nutr VERSION 0.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(OUTPUT_FORMAT "root" CACHE STRING "Determine the format of the output. Possible choices: 'root' (default), 'csv', and 'xml'.")

set(GEOMETRY_DIR "clover_array/02_16_2021_to_04_10_2021" CACHE STRING "Select directory in ${PROJECT_SOURCE_DIR}/src/geometry that contains the desired geometry (default: 'clover_array/02_16_2021_to_04_10_2021').")

set(PRIMARY_GENERATOR_DIR "gps" CACHE STRING "Select directory in `${PROJECT_SOURCE_DIR}/src/fundamentals/primary_generator` that contains the desired primary generator Possible choices: 'gps' (default), 'angcorr'.")

set(PRODUCTION_CUT_LOW_KEV "0.99" CACHE STRING "Set the lower energy limit of the production cut for gammas, electrons/positrons and protons in keV (default: '0.99', i.e. use default production cut of G4EmLivermorePolarizedPhysics). A straightforward way to view the current production cuts is the '/run/particle/dumpCutValues' macro command.")

set(SENSITIVE_DETECTOR_DIR "event" CACHE STRING "Select directory in `${PROJECT_SOURCE_DIR}/src/sensitive_detector` that contains the desired sensitive detector. Possible choices: `edep`, `event` (default), `flux`, `tracker`.")

set(UPDATE_FREQUENCY "10000" CACHE STRING "Determine the number of events since the last update after which a new update about the progress of the simulation is printed on the command line (default: 10000).")

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(src/detectors)
add_subdirectory(src/fundamentals)
add_subdirectory(src/geometry)
add_subdirectory(src/geometry/materials)
if(<variable|${GEOMETRY_DIR}> MATCHES "clover_array")
	add_subdirectory(src/geometry/clover_array/array)
endif()
add_subdirectory(src/geometry/${GEOMETRY_DIR})
add_subdirectory(src/physics)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/primary_generator/${PRIMARY_GENERATOR_DIR})
add_subdirectory(src/sensitive_detector)
add_subdirectory(src/sensitive_detector/${SENSITIVE_DETECTOR_DIR})

add_executable(nutr src/fundamentals/nutr.cc)
target_include_directories(nutr PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals ${PROJECT_SOURCE_DIR}/include/geometry ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(nutr actionInitialization detectorConstruction physics ${Geant4_LIBRARIES})

set(MACROS_ESSENTIAL init_vis.mac vis.mac)
foreach(macro ${MACROS_ESSENTIAL})
	configure_file(${PROJECT_SOURCE_DIR}/macros/${macro} ${PROJECT_BINARY_DIR}/${macro} COPYONLY)
endforeach()

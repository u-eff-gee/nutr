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

include_directories(${PROJECT_SOURCE_DIR}/include/physics ${PROJECT_BINARY_DIR}/include/physics)

if(DEFINED ENV{G4LENDDATA})
    set(USE_LENDGAMMANUCLEAR "true" CACHE STRING "Use LENDGammaNuclear option of G4EmExtraPhysics.")
    set(HADRON_ELASTIC "G4HadronElasticPhysicsLEND" CACHE STRING "Use G4HadronElasticPhysicsLEND.")
    set(HADRON_INELASTIC "G4HadronPhysicsShieldingLEND" CACHE STRING "Use G4HadronPhysicsShieldingLEND")
else()
    message(STATUS "Warning: Low-energy nuclear data (LEND) not found, using fallback physics models.")
    set(USE_LENDGAMMANUCLEAR "false" CACHE STRING "Do not use LENDGammaNuclear option of G4EmExtraPhysics.")
    set(HADRON_ELASTIC "G4HadronElasticPhysicsHP" CACHE STRING "Use G4HadronElasticPhysicsHP.")
    set(HADRON_INELASTIC "G4HadronPhysicsShielding" CACHE STRING "Use G4HadronPhysicsShielding")
endif()
configure_file(${PROJECT_SOURCE_DIR}/src/physics/Physics.cc.in ${PROJECT_BINARY_DIR}/src/physics/Physics.cc)

option(USE_HADRON_PHYSICS "Include hadron physics lists (default: ON). Excluding hadron physics can speed up the startup of the simulation. This is useful, for example, when you only want to visualize the geometry. It might speed up the actual simulation as well, but sometimes hadron interactions cannot be neglected." ON)
configure_file(${PROJECT_SOURCE_DIR}/include/physics/PhysicsConfig.hh.in ${PROJECT_BINARY_DIR}/include/physics/PhysicsConfig.hh)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

add_library(physics ${PROJECT_BINARY_DIR}/src/physics/Physics.cc)

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

include_directories(${PROJECT_SOURCE_DIR}/include/sensitive_detector/flux)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})

add_library(DetectorHit DetectorHit.cc)
target_include_directories(DetectorHit PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector)
target_link_libraries(DetectorHit nDetectorHit)

add_library(SensitiveDetector SensitiveDetector.cc)
target_link_libraries(SensitiveDetector nSensitiveDetector DetectorHit)

add_library(tupleManager TupleManager.cc)
target_link_libraries(tupleManager analysisManager DetectorHit)

add_library(eventAction EventAction.cc)
target_include_directories(eventAction PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector)
target_link_libraries(eventAction nEventAction tupleManager)

add_library(runAction RunAction.cc)
target_link_libraries(runAction nRunAction tupleManager)
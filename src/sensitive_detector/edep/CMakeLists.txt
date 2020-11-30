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

include_directories(${PROJECT_SOURCE_DIR}/include/sensitive_detector/edep)

find_package(Geant4 REQUIRED)
link_libraries(${Geant4_LIBRARIES})
include(${Geant4_USE_FILE})

add_library(DetectorHit DetectorHit.cc)
target_include_directories(DetectorHit PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector)
target_link_libraries(DetectorHit nDetectorHit)
target_compile_options(DetectorHit PUBLIC -Wall -Wextra)

add_library(SensitiveDetector SensitiveDetector.cc)
target_link_libraries(SensitiveDetector nSensitiveDetector DetectorHit)
target_compile_options(SensitiveDetector PUBLIC -Wall -Wextra)

add_library(tupleManager TupleManager.cc)
target_include_directories(tupleManager PUBLIC ${PROJECT_BINARY_DIR}/include/sensitive_detector)
target_link_libraries(tupleManager analysisManager DetectorHit)
target_compile_options(tupleManager PUBLIC -Wall -Wextra)

add_library(eventAction EventAction.cc)
target_include_directories(eventAction PUBLIC ${PROJECT_SOURCE_DIR}/include/sensitive_detector)
target_link_libraries(eventAction nEventAction tupleManager)
target_compile_options(eventAction PUBLIC -Wall -Wextra)

add_library(runAction RunAction.cc)
target_link_libraries(runAction nRunAction tupleManager)
target_compile_options(runAction PUBLIC -Wall -Wextra)
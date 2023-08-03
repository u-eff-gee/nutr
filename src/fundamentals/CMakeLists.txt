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

add_library(actionInitialization ActionInitialization.cc NutrMessenger.cc)
target_include_directories(actionInitialization PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/gps ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(actionInitialization eventAction primaryGeneratorAction nRunAction ${Geant4_LIBRARIES})

add_library(actionInitialization_angcorr ActionInitialization.cc NutrMessenger.cc)
target_include_directories(actionInitialization_angcorr PUBLIC ${PROJECT_SOURCE_DIR}/include/primary_generator/angcorr ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR})
target_link_libraries(actionInitialization_angcorr PUBLIC eventAction primaryGeneratorActionAngCorr nRunAction)
target_link_libraries(actionInitialization_angcorr PRIVATE cascadeRejectionSampler ${Geant4_LIBRARIES})

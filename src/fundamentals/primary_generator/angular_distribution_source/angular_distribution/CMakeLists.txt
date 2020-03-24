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

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include/fundamentals/primary_generator/angular_distribution_source/angular_distribution)

add_library(fCoefficient FCoefficient.cc)
target_link_libraries(fCoefficient ${GSL_LIBRARIES})

add_library(avCoefficient AvCoefficient.cc)
target_link_libraries(avCoefficient fCoefficient)

add_library(w_dir_dir W_dir_dir.cc)
target_link_libraries(w_dir_dir avCoefficient)

enable_testing()
add_executable(test_f_coefficient test_f_coefficient.cc)
target_link_libraries(test_f_coefficient fCoefficient)
add_test(test_f_coefficient test_f_coefficient)

add_executable(test_w_dir_dir test_w_dir_dir.cc)
target_link_libraries(test_w_dir_dir w_dir_dir)
add_test(test_w_dir_dir test_w_dir_dir)
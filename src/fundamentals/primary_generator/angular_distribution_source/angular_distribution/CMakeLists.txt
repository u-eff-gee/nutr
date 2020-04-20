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

add_library(w_dir_dir SHARED W_dir_dir.cc)
target_link_libraries(w_dir_dir avCoefficient)
configure_file(w_dir_dir.py.in w_dir_dir.py)

add_library(kappa_coefficient KappaCoefficient.cc)
target_link_libraries(kappa_coefficient ${GSL_LIBRARIES})

add_library(alphavCoefficient AlphavCoefficient.cc)
target_link_libraries(alphavCoefficient fCoefficient kappa_coefficient)

add_library(w_pol_dir SHARED W_pol_dir.cc)
target_link_libraries(w_pol_dir alphavCoefficient avCoefficient w_dir_dir)
configure_file(w_pol_dir.py.in w_pol_dir.py)

add_library(spherePointSampler SpherePointSampler.cc)
target_link_libraries(spherePointSampler ${GSL_LIBRARIES})

enable_testing()
add_executable(test_f_coefficient test_f_coefficient.cc)
target_link_libraries(test_f_coefficient fCoefficient)
add_test(test_f_coefficient test_f_coefficient)

add_executable(test_w_dir_dir test_w_dir_dir.cc)
target_link_libraries(test_w_dir_dir w_dir_dir)
add_test(test_w_dir_dir test_w_dir_dir)

add_executable(test_kappa_coefficient test_kappa_coefficient.cc)
target_link_libraries(test_kappa_coefficient kappa_coefficient)
add_test(test_kappa_coefficient test_kappa_coefficient)

add_executable(test_w_pol_dir test_w_pol_dir.cc)
target_link_libraries(test_w_pol_dir w_pol_dir ${GSL_LIBRARIES})
add_test(test_w_pol_dir test_w_pol_dir)

# Integration of ROOT libraries using information from the manual:
# "Integrate ROOT into my project with CMake"
# https://root.cern.ch/how/integrate-root-my-project-cmake (04/09/2020)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT COMPONENTS RGL)
if(ROOT_FOUND)
    add_executable(test_plots_tgl test_plots_tgl.cc)
    target_link_libraries(test_plots_tgl w_pol_dir ${ROOT_LIBRARIES})
    add_test(test_plots_tgl test_plots_tgl)

    add_executable(test_plots_tpolymarker3d test_plots_tpolymarker3d.cc)
    target_link_libraries(test_plots_tpolymarker3d w_pol_dir spherePointSampler ${ROOT_LIBRARIES})
    add_test(test_plots_tpolymarker3d test_plots_tpolymarker3d)    
endif()

add_executable(test_elliptic_integral test_elliptic_integral.cc)
target_link_libraries(test_elliptic_integral spherePointSampler ${GSL_LIBRARIES})
add_test(test_elliptic_integral test_elliptic_integral)

add_executable(test_spiral_segment test_spiral_segment.cc)
target_link_libraries(test_spiral_segment spherePointSampler ${GSL_LIBRARIES})
add_test(test_spiral_segment test_spiral_segment)

add_executable(test_find_c test_find_c.cc)
target_link_libraries(test_find_c spherePointSampler)
add_test(test_find_c test_find_c)

add_executable(test_find_Theta_j test_find_Theta_j.cc)
target_link_libraries(test_find_Theta_j spherePointSampler)
add_test(test_find_Theta_j test_find_Theta_j)

configure_file(test_plots_matplotlib.py.in test_plots_matplotlib.py)
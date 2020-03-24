find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/src/fundamentals/primary_generator/angular_distribution_source/angular_distribution/Doxyfile.in Doxyfile)
	add_custom_target(doc_doxygen ALL COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)
endif(DOXYGEN_FOUND)

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
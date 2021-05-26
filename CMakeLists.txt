cmake_minimum_required(VERSION 3.0.0)
project(libgramas VERSION 0.1.0)

macro(build_test test_name)
	add_executable(${test_name} test/${test_name}.c)
	target_link_libraries(${test_name} libgramas)
	add_test(${test_name} ${test_name})
endmacro()

include(CTest)
enable_testing()

include_directories(include)
add_library(libgramas SHARED
	buf.c
	cmd_parse.c
	gr_str.c
	hash_map.c
	matrix.c
	ptr_array.c
	space.c
	utf8.c)

build_test(tst_hash_map_put)
build_test(tst_matrix_mul)
build_test(tst_ptr_arr_add)
build_test(tst_space_distance)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(libgramas VERSION 0.1.0 LANGUAGES C)

set(DEFAULT_SRC
	buf.c
	cmd_parse.c
	field.c
	gr_str.c
	hash_map.c
	matrix.c
	ptr_array.c
	space.c
	str.c
	utf8.c
)
macro(build_test test_name)
	add_executable(${test_name} test/${test_name}.c)
	target_link_libraries(${test_name} ${PROJECT_NAME})
	add_test(${test_name} ${test_name})
endmacro()

include(CTest)
enable_testing()

include_directories(include)
add_library(${PROJECT_NAME} SHARED ${DEFAULT_SRC})

if (UNIX)
	target_link_libraries(${PROJECT_NAME} -lm)
endif(UNIX)

build_test(tst_hash_map_put)
build_test(tst_matrix_mul)
build_test(tst_ptr_arr_add)
build_test(tst_space_distance)
build_test(tst_rdwr_bele)
build_test(tst_buf2str)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

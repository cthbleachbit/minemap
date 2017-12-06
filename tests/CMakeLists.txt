set(BASE_FILES ../src/mc_nbt.c ../src/mc_map.c ../src/utils.c ../src/mc_endian.c)

include_directories(../src)

add_executable(endian_t1 endian_t1.c ${BASE_FILES})
add_executable(endian_t2 endian_t2.c ${BASE_FILES})
add_test(endian_test_1 endian_t1)
add_test(endian_test_2 endian_t2)

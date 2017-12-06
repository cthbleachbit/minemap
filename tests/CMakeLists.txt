set(BASE_FILES ../src/mc_nbt.c ../src/mc_map.c ../src/utils.c ../src/mc_nbt.h ../src/mc_map.h ../src/utils.h ../src/mc_endian.h)

add_executable(endian_t1 endian_t1.c ${BASE_FILES})
add_executable(endian_t2 endian_t2.c)
add_test(endian_test_1 endian_t1)
add_test(endian_test_2 endian_t2)

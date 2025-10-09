add_executable(imu_test imu_test.c)
target_link_libraries(imu_test PRIVATE imu)
target_compile_features(imu_test PRIVATE c_std_11)

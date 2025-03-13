add_executable(constexpr_md5_unit_tests main.cc)

target_compile_features(constexpr_md5_unit_tests
    PRIVATE
        cxx_std_23
)

target_include_directories(constexpr_md5_unit_tests PRIVATE ../include)

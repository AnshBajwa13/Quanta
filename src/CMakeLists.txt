add_library(quant_engine_core STATIC)

target_sources(
    quant_engine_core
    PRIVATE
        core/version.cpp
)

target_include_directories(
    quant_engine_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
    quant_engine_core
    PUBLIC
        cxx_std_20
)
add_executable(quant-engine-tests)

target_sources(
    quant-engine-tests
    PRIVATE
        unit/version_test.cpp
)

target_link_libraries(
    quant-engine-tests
    PRIVATE
        quant_engine_core
)

add_test(
    NAME version_test
    COMMAND quant-engine-tests
)
add_executable(quant-engine)

target_sources(
    quant-engine
    PRIVATE
        main.cpp
)

target_link_libraries(
    quant-engine
    PRIVATE
        quant_engine_core
)
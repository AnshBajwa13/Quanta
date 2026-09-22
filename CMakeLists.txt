cmake_minimum_required(VERSION 3.25)

project(
    quant_engine
    VERSION 0.1.0
    DESCRIPTION "Low-latency quantitative trading engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------------------------

if(MSVC)
    add_compile_options(
        /W4
        /permissive-
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
    )
endif()

# ---------------------------------------------------------------------------
# Core library
# ---------------------------------------------------------------------------

add_library(quant_core STATIC
    src/core/version.cpp
    src/market/market_data_dispatcher.cpp
    src/market/replay_feed.cpp
    src/strategy/strategy_engine.cpp
    src/risk/kill_switch.cpp
    src/risk/risk_manager.cpp
    src/portfolio/portfolio_manager.cpp
    src/execution/matching_engine.cpp
    src/execution/simulator.cpp
    src/execution/simulator_gateway.cpp
    src/execution/order_manager.cpp
    src/execution/order_router.cpp
    src/adapters/upstox/upstox_market_data_feed.cpp
    src/adapters/upstox/upstox_execution_gateway.cpp
)

target_include_directories(
    quant_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
    quant_core
    PUBLIC
        cxx_std_20
)

# ---------------------------------------------------------------------------
# Engine executable
# ---------------------------------------------------------------------------

add_executable(quant-engine
    apps/quant-engine/main.cpp
)

target_link_libraries(
    quant-engine
    PRIVATE
        quant_core
)

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------

enable_testing()

add_executable(quant-core-tests
    tests/units/version_test.cpp
)

target_link_libraries(
    quant-core-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_core_tests
    COMMAND quant-core-tests
)

add_executable(quant-core-types-tests
    tests/units/core_types_test.cpp
)

target_link_libraries(
    quant-core-types-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_core_types_tests
    COMMAND quant-core-types-tests
)

add_executable(quant-execution-types-tests
    tests/units/execution_types_test.cpp
)

target_link_libraries(
    quant-execution-types-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_execution_types_tests
    COMMAND quant-execution-types-tests
)

add_executable(quant-instrument-tests
    tests/units/instrument_test.cpp
)

target_link_libraries(
    quant-instrument-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_instrument_tests
    COMMAND quant-instrument-tests
)

add_executable(quant-market-tick-tests
    tests/units/market_tick_test.cpp
)

target_link_libraries(
    quant-market-tick-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_market_tick_tests
    COMMAND quant-market-tick-tests
)

add_executable(quant-trade-tests
    tests/units/trade_test.cpp
)

target_link_libraries(
    quant-trade-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_trade_tests
    COMMAND quant-trade-tests
)

add_executable(quant-order-tests
    tests/units/order_test.cpp
)

target_link_libraries(
    quant-order-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_order_tests
    COMMAND quant-order-tests
)

add_executable(quant-fill-tests
    tests/units/fill_test.cpp
)

target_link_libraries(
    quant-fill-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_fill_tests
    COMMAND quant-fill-tests
)

add_executable(quant-execution-gateway-tests
    tests/units/execution_gateway_test.cpp
)

target_link_libraries(
    quant-execution-gateway-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_execution_gateway_tests
    COMMAND quant-execution-gateway-tests
)

add_executable(quant-matching-engine-tests
    tests/units/matching_engine_test.cpp
)

target_link_libraries(
    quant-matching-engine-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_matching_engine_tests
    COMMAND quant-matching-engine-tests
)

add_executable(quant-simulator-tests
    tests/units/simulator_test.cpp
)

target_link_libraries(
    quant-simulator-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_simulator_tests
    COMMAND quant-simulator-tests
)

add_executable(quant-spsc-queue-tests
    tests/units/spsc_queue_test.cpp
)

target_link_libraries(
    quant-spsc-queue-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_spsc_queue_tests
    COMMAND quant-spsc-queue-tests
)

add_executable(quant-order-book-tests
    tests/units/order_book_test.cpp
)

target_link_libraries(
    quant-order-book-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_order_book_tests
    COMMAND quant-order-book-tests
)

add_executable(quant-market-data-tests
    tests/units/market_data_test.cpp
)

target_link_libraries(
    quant-market-data-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_market_data_tests
    COMMAND quant-market-data-tests
)

add_executable(quant-replay-feed-tests
    tests/units/replay_feed_test.cpp
)

target_link_libraries(
    quant-replay-feed-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_replay_feed_tests
    COMMAND quant-replay-feed-tests
)

add_executable(quant-strategy-tests
    tests/units/strategy_test.cpp
)

target_link_libraries(
    quant-strategy-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_strategy_tests
    COMMAND quant-strategy-tests
)

add_executable(quant-risk-tests
    tests/units/risk_test.cpp
)

target_link_libraries(
    quant-risk-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_risk_tests
    COMMAND quant-risk-tests
)

add_executable(quant-portfolio-tests
    tests/units/portfolio_test.cpp
)

target_link_libraries(
    quant-portfolio-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_portfolio_tests
    COMMAND quant-portfolio-tests
)

add_executable(quant-order-manager-tests
    tests/units/order_manager_test.cpp
)

target_link_libraries(
    quant-order-manager-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_order_manager_tests
    COMMAND quant-order-manager-tests
)

add_executable(quant-order-router-tests
    tests/units/order_router_test.cpp
)

target_link_libraries(
    quant-order-router-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_order_router_tests
    COMMAND quant-order-router-tests
)

add_executable(quant-upstox-adapter-tests
    tests/units/upstox_adapter_test.cpp
)

target_link_libraries(
    quant-upstox-adapter-tests
    PRIVATE
        quant_core
)

add_test(
    NAME quant_upstox_adapter_tests
    COMMAND quant-upstox-adapter-tests
)
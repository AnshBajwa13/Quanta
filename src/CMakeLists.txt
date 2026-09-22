add_library(quant_engine_core STATIC)

target_sources(
    quant_engine_core
    PRIVATE
        core/version.cpp
        market/market_data_dispatcher.cpp
        market/replay_feed.cpp
        strategy/strategy_engine.cpp
        risk/kill_switch.cpp
        risk/risk_manager.cpp
        portfolio/portfolio_manager.cpp
        execution/matching_engine.cpp
        execution/simulator.cpp
        execution/simulator_gateway.cpp
        execution/order_manager.cpp
        execution/order_router.cpp
        adapters/upstox/upstox_market_data_feed.cpp
        adapters/upstox/upstox_execution_gateway.cpp
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
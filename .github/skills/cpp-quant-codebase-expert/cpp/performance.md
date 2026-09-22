# C++ Performance

Analyze:
- algorithmic complexity
- allocations
- copies/moves
- cache locality
- branch prediction
- data layout
- SIMD/vectorization
- inlining
- virtual calls
- synchronization
- NUMA
- false sharing
- syscalls
- I/O

Prefer measurement using:
- benchmarks
- profilers
- perf
- flamegraphs
- compiler optimization reports
- sanitizers where appropriate

For low-latency code consider p50/p95/p99/p99.9 latency and tail behavior, not only averages.

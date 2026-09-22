# C++ Concurrency

Analyze:
- mutex ownership
- lock ordering
- critical sections
- atomics
- acquire/release/relaxed/seq_cst
- happens-before
- data races
- condition variables
- thread pools
- task queues
- futures/promises
- coroutines
- lock-free structures
- ABA problems
- false sharing
- cache coherence

For a suspected race:
1. Identify shared state.
2. Identify every reader/writer.
3. Identify synchronization.
4. Establish happens-before relationships.
5. Determine whether memory ordering is sufficient.
6. Propose the smallest correct fix.

Never use sleeps as synchronization.

# Modern C++ Expertise

Master C++11 through C++23 and relevant C++26 facilities.

## Core language
- RAII and deterministic destruction
- Rule of 0/3/5
- copy/move construction and assignment
- value categories
- temporary materialization
- guaranteed copy elision
- perfect forwarding
- reference collapsing
- lifetime extension
- const-correctness
- noexcept and exception guarantees

## STL and generic facilities
- containers
- algorithms
- iterators
- ranges/views
- allocators
- memory resources
- type traits
- constexpr/consteval
- concepts/requires
- fold expressions
- CTAD

## Advanced C++
- templates
- SFINAE
- CRTP
- ADL
- overload resolution
- coroutines
- modules
- ABI/object model

Always reason from actual language semantics and the repository's compiler/toolchain.

Inspect ownership and lifetime before recommending pointer changes.

Prefer simple explicit designs unless repository constraints justify advanced techniques.

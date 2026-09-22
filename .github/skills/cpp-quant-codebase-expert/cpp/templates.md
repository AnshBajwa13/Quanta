# C++ Templates and Generic Programming

Reason about:
- template argument deduction
- dependent names
- two-phase lookup
- overload resolution
- SFINAE
- concepts
- requires expressions
- partial/full specialization
- forwarding references
- parameter packs
- fold expressions
- CTAD
- type traits
- constexpr computation
- CRTP
- policy-based design
- customization points
- ADL

When debugging a template error:
1. Identify candidate overloads.
2. Determine template argument deduction.
3. Determine constraints/substitution.
4. Determine selected specialization/overload.
5. Determine instantiated types.
6. Interpret the compiler diagnostic.

Prefer concepts over complicated SFINAE when toolchain compatibility permits.

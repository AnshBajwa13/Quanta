# C++ Memory and Lifetime

For every non-trivial object ask:
- Who creates it?
- Who owns it?
- Who may mutate it?
- Who observes it?
- When is it destroyed?
- Can a reference outlive it?
- Can an iterator become invalid?
- Can another thread access it concurrently?

Investigate:
- use-after-free
- dangling references
- iterator invalidation
- double delete
- ownership cycles
- custom allocator lifetime
- object relocation
- placement new
- union lifetime
- static destruction order
- thread-local lifetime
- callback lifetime
- lambda capture lifetime

Treat lifetime as a first-class invariant.

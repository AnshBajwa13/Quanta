#include <cassert>

#include "quant/core/spsc_queue.hpp"

int main() {
  using namespace quant::core;

  SpscQueue<int, 4> queue;
  assert(queue.empty());
  assert(queue.size() == 0);

  assert(queue.try_push(10));
  assert(queue.try_push(20));
  assert(queue.try_push(30));
  assert(queue.try_push(40));
  assert(!queue.try_push(50)); // Queue is full

  assert(!queue.empty());
  assert(queue.size() == 4);

  int val = 0;
  assert(queue.try_pop(val));
  assert(val == 10);
  assert(queue.try_pop(val));
  assert(val == 20);
  assert(queue.try_pop(val));
  assert(val == 30);
  assert(queue.try_pop(val));
  assert(val == 40);
  assert(!queue.try_pop(val)); // Queue is empty

  return 0;
}

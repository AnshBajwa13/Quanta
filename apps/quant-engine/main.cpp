#include <iostream>

#include "quant/core/version.hpp"

int main() {
  std::cout << "quant-engine v" << quant::core::version_major() << '\n';

  return 0;
}
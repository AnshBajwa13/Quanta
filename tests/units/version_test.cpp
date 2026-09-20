#include <cassert>

#include "quant/core/version.hpp"

int main() {
    assert(quant::core::version_major() == 0);
    return 0;
}
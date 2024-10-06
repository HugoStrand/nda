
#include <nda/nda.hpp>

void memory_leak(size_t size) {
  nda::vector<dcomplex> tmp(size);
  tmp += 1.0;
}

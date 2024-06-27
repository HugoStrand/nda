// Copyright (c) 2022-2023 Simons Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors: Miguel Morales, Nils Wentzell

#pragma once

#include <cstdlib>

#include "address_space.hpp"
#include "../macros.hpp"
#include "../traits.hpp"

namespace nda::mem {

  template <AddressSpace AdrSp>
  void *malloc(size_t size) {
    check_adr_sp_valid<AdrSp>();
    static_assert(nda::have_device == nda::have_cuda, "Adjust function for new device types");

    void *ptr = nullptr;
    if constexpr (AdrSp == Host) {
      ptr = std::malloc(size);
    } else if constexpr (AdrSp == Device) {
      device_error_check(cudaMalloc((void **)&ptr, size), "cudaMalloc");
    } else { // Unified
      device_error_check(cudaMallocManaged((void **)&ptr, size), "cudaMallocManaged");
    }
    return ptr;
  }

  template <AddressSpace AdrSp>
  void free(void *p) {
    check_adr_sp_valid<AdrSp>();
    static_assert(nda::have_device == nda::have_cuda, "Adjust function for new device types");

    if constexpr (AdrSp == Host) {
      std::free(p);
    } else { // Device or Unified
      device_error_check(cudaFree(p), "cudaFree");
    }
  }

} // namespace nda::mem

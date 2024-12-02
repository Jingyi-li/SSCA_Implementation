//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_c_2 {
public:

  alignas(16) int (&fft_lut_tw2)[64];

  // Constructor:
  ssr_fft_fp_c_2(int (&lut2)[64]);

  // Run:
  void run( input_buffer_1d<int,2048,0>& __restrict inputx,
            output_buffer_1d<int,2048>& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_c_2::run );
    REGISTER_PARAMETER( fft_lut_tw2 );
  }
};


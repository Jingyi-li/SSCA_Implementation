//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_a_xc_2_stream {
public:
  // Constructor:
  ssr_fft_fp_a_xc_2_stream(int (&winbuff0)[1024], int (&winbuff1)[1024], int (&window_coeff)[64]);
  alignas(16) int (&window_coeff)[64];
  alignas(16) int (&winbuff0)[1024];
  alignas(16) int (&winbuff1)[1024];
  void stream_out (cfloat * restrict px0, cfloat * restrict px1,  output_window<cfloat>* restrict py0);
  // Run:
  void run( input_buffer_1d<int,1024,0>& __restrict inputx0,
            input_buffer_1d<int,1024,0>& __restrict inputx1,
            output_buffer_1d<int,2048>& __restrict outputy,
            output_window<cfloat>* __restrict outputy1 );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_a_xc_2_stream::run );
    REGISTER_PARAMETER( window_coeff );
    REGISTER_PARAMETER( winbuff0 );
    REGISTER_PARAMETER( winbuff1 );
  }
};


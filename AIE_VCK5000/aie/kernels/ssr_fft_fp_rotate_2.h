//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_rotate_2 {
public:

  int idx;
  int idxloop;

  // // 1536 = 32*48 twiddle rotate factors
  // alignas(16) int (&twrom)[1536];
  alignas(16) int (&fft_phrot_buf)[2048];

  // Constructor:
  // ssr_fft_fp_rotate( int (&twrom_i)[1536], int (&fft_phrot_buf_i)[2048] );
  ssr_fft_fp_rotate_2(int (&fft_phrot_buf_i)[2048] );

  void fp_elem_mul (cfloat * restrict x1, cfloat * restrict x2, cfloat * restrict y1, cfloat * restrict y2);
  void fp_dds(int * rom);

  // Run:
  void run( input_buffer_1d<int,1024,0>& __restrict x0,
            input_buffer_1d<int,1024,0>& __restrict x1,
            input_buffer_1d<int, 48, 0>& __restrict tw,
            output_buffer_1d<int,1024>& __restrict y0,
            output_buffer_1d<int,1024>& __restrict y1 );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_rotate_2::run );
    // REGISTER_PARAMETER( twrom );
    REGISTER_PARAMETER( fft_phrot_buf );
  }
};


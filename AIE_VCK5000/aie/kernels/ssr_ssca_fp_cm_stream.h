//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_ssca_fp_cm_stream {
public:

  alignas(16) int (&cbuff)[32];
  // Constructor:
  ssr_ssca_fp_cm_stream(int (&cbuff_i)[32]);
  void save_buff(cfloat * restrict cbuff_i, input_window<cfloat> * restrict input);
  
  

  // Run:
  void run( input_buffer_1d<int,2048,0>& __restrict inputx0,
            input_window<cfloat>* __restrict inputx1,
            output_buffer_1d<int,1024>& __restrict outputy0,
            output_buffer_1d<int,1024>& __restrict outputy1 );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_ssca_fp_cm_stream::run );
    REGISTER_PARAMETER( cbuff );
  }
};


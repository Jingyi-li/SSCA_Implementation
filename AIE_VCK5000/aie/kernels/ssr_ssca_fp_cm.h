//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_ssca_fp_cm {
public:

  // alignas(16) int (&window_coeff)[64];
  // Constructor:
  ssr_ssca_fp_cm();
  
  

  // Run:
  void run( input_buffer_1d<int,2048,0>& __restrict inputx0,
            input_buffer_1d<int,32,0>& __restrict inputx1,
            output_buffer_1d<int,1024>& __restrict outputy0,
            output_buffer_1d<int,1024>& __restrict outputy1 );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_ssca_fp_cm::run );
  }
};


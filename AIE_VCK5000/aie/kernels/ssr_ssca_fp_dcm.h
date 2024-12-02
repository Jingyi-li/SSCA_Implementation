//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_ssca_fp_dcm {
public:
  int idx;
  int idxloop;
  alignas(16) int (&dcm_lut_tw3)[128];
  alignas(16) int (&lutexp)[128];
  
  // Constructor:
  ssr_ssca_fp_dcm(int (&lutexp_i)[128], int (&exp_buff)[128]);

  void dcm_coeff(cfloat * exp_buff, int idx_i);


  // Run:
  void run( input_buffer_1d<int,2048,0>& __restrict inputx,
            output_buffer_1d<int,2048>& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_ssca_fp_dcm::run );
    REGISTER_PARAMETER( lutexp );
    REGISTER_PARAMETER( dcm_lut_tw3 );
  }
};


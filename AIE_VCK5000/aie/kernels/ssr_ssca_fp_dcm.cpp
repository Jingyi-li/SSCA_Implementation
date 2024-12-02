//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_ssca_fp_dcm.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_ssca_fp_dcm::ssr_ssca_fp_dcm(int (&lutexp_i)[128] , int (&exp_buff)[128] )
  :lutexp(lutexp_i), dcm_lut_tw3(exp_buff)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
  // idx = 2;
}

// ------------------------------------------------------------
// fp_dds
// ------------------------------------------------------------
inline __attribute__((always_inline))
void ssr_ssca_fp_dcm::dcm_coeff(cfloat * rom, int idx_i)
{
  cfloat * restrict tw = (cfloat * restrict) dcm_lut_tw3;
  for(int i = 0; i< 64; i++){
    *tw++ = rom[(i*idx_i)&63];
  }
}
// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_ssca_fp_dcm::run( input_buffer_1d<int,2048,0>& __restrict inputx,
                        output_buffer_1d<int,2048>& __restrict outputy )
{
  cfloat * restrict xbuf =  (cfloat * restrict)inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict)outputy.data();

  cfloat * restrict twb = (cfloat * restrict) dcm_lut_tw3;

  // Radix-2 stages
  if (idxloop==0) dcm_coeff((cfloat * restrict) lutexp, idx);
  idx = (idx + (idxloop == 0)) & 63;
  idxloop = (idxloop + 1) & 63;
  ssca_dc_multi(  xbuf,   twb,  1024,   ybuf); 
}



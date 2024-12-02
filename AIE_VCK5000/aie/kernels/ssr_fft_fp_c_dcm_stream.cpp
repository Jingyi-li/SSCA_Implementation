//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_c_dcm_stream.h"
#include "fft_stages.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_c_dcm_stream::ssr_fft_fp_c_dcm_stream( int (&fftbuf_i)[2048], int (&lut2)[64], int (&lutexp_i)[128] , int (&exp_buff)[128] )
  : fftbuf(fftbuf_i), fft_lut_tw2(lut2), lutexp(lutexp_i), dcm_lut_tw3(exp_buff)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// fp_dds
// ------------------------------------------------------------
inline __attribute__((always_inline))
void ssr_fft_fp_c_dcm_stream::dcm_coeff(cfloat * rom, int idx_i)
{
  cfloat * restrict tw = (cfloat * restrict) dcm_lut_tw3;
  for(int i = 0; i< 64; i++){
    *tw++ = rom[(i*idx_i)&63];
    // cfloat test = rom[(i*idx_i)&63];
    // printf("tw[%d] = (%f,%f)\n",i,test.real,test.imag);
  }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_c_dcm_stream::run( input_buffer_1d<int,2048,0>& __restrict inputx,
                            input_window<cfloat>* __restrict sin,
                            output_buffer_1d<int,2048>& __restrict outputy,
                            output_window<cfloat>* __restrict sout )
{
  cfloat * restrict xbuf =  (cfloat * restrict)inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict)outputy.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw2;
  cfloat * restrict twb = (cfloat * restrict) dcm_lut_tw3;

  // Radix-2 stages
  stage5_radix2_dit(  xbuf,   twa,  1024,   (cfloat * restrict)    fftbuf);  // s5
  if (idxloop==0) dcm_coeff((cfloat * restrict) lutexp, idx);
  idx = (idx + (idxloop == 0)) & 63;
  idxloop = (idxloop + 1) & 63;
  ssca_dc_multi    (  (cfloat * restrict) fftbuf, twb,  1024,        ybuf);
  auto shift_register = window_readincr_v<16>(sin);
  window_writeincr(sout, shift_register);
}




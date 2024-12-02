//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_b_stream.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_b_stream::ssr_fft_fp_b_stream( int (&fftbuf_i)[2048], int (&lut0)[16], int (&lut1)[32] )
  : fftbuf(fftbuf_i), fft_lut_tw0(lut0), fft_lut_tw1(lut1)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

inline __attribute__((always_inline)) 
void ssr_fft_fp_b_stream::passdata(input_window<cfloat>* restrict sin, output_window<cfloat>* restrict sout)
{
  auto shift_register = window_readincr_v<16>(sin);
  window_writeincr(sout, shift_register);
}
// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_b_stream::run(  input_buffer_1d<int,2048,0>& __restrict inputx,
                                input_window<cfloat>* __restrict sin,
                                output_buffer_1d<int,2048>& __restrict outputy,
                                output_window<cfloat>* __restrict sout )
{
  cfloat * restrict xbuf =  (cfloat * restrict)  inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict) outputy.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw0;
  cfloat * restrict twb = (cfloat * restrict) fft_lut_tw1;

  stage3_radix2_dit(  xbuf,   twa,  1024,   (cfloat * restrict) fftbuf);  // s3
  stage4_radix2_dit(  (cfloat * restrict) fftbuf, twb,  1024,       ybuf);    // s4
  passdata(sin, sout);
}



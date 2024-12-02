//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_c_2.h"
#include "fft_stages.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_c_2::ssr_fft_fp_c_2(int (&lut2)[64])
  :fft_lut_tw2(lut2)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_c_2::run( input_buffer_1d<int,2048,0>& __restrict inputx,
                        output_buffer_1d<int,2048>& __restrict outputy )
{
  cfloat * restrict xbuf =  (cfloat * restrict)inputx.data();
  cfloat * restrict ybuf =  (cfloat * restrict)outputy.data();

  cfloat * restrict twa = (cfloat * restrict) fft_lut_tw2;

  // Radix-2 stages
  stage5_radix2_dit(  xbuf,   twa,  1024,   ybuf);  // s5
}



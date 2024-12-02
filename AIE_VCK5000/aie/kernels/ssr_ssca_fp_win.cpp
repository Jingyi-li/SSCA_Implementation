//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_ssca_fp_win.h"
#include "fft_stages.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_ssca_fp_win::ssr_ssca_fp_win(int (&window_coeff_i)[64])
  : window_coeff(window_coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_ssca_fp_win::run( input_buffer_1d<int,1024,0>& __restrict inputx0,
                        input_buffer_1d<int,1024,0>& __restrict inputx1,
                        output_buffer_1d<int,1024>& __restrict outputy0,
                        output_buffer_1d<int,1024>& __restrict outputy1 )
{
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  cfloat * restrict xbuf1 =  (cfloat * restrict) inputx1.data();
  cfloat * restrict ybuf0 =  (cfloat * restrict) outputy0.data();
  cfloat * restrict ybuf1 =  (cfloat * restrict) outputy1.data();

  cfloat * restrict tw = (cfloat * restrict) window_coeff;

  // Windoe coeff
  window_chebwin   (  xbuf0, xbuf1, tw, 1024, ybuf0, ybuf1);

  // // Radix-2 stages
  // stage0_radix2_dit(  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1,      1024,    ybuff);         // s0
  // stage1_radix2_dit(     ybuff,     1024,  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1);  // s1
  // stage2_radix2_dit(  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1,      1024,    ybuff);         // s2

}



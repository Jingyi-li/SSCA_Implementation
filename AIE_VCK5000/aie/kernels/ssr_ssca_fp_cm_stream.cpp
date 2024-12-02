//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_ssca_fp_cm_stream.h"
#include "fft_stages.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_ssca_fp_cm_stream::ssr_ssca_fp_cm_stream(int (&cbuff_i)[32])
:cbuff(cbuff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

inline __attribute__((always_inline)) 
void ssr_ssca_fp_cm_stream::save_buff(cfloat * restrict cbuff_i, input_window<cfloat>* restrict input)
{
  cfloat * restrict cx = (cfloat * restrict) cbuff_i; 
  for (int i = 0; i < 16; i++){
    *cx++ = window_readincr(input);
  }
}
// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_ssca_fp_cm_stream::run( input_buffer_1d<int,2048,0>& __restrict inputx0,
                        input_window<cfloat>* __restrict inputx1,
                        output_buffer_1d<int,1024>& __restrict outputy0,
                        output_buffer_1d<int,1024>& __restrict outputy1 )
{
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  // cfloat * restrict cbuff =  (cfloat * restrict) inputx1.data();
  cfloat * restrict ybuf0 =  (cfloat * restrict) outputy0.data();
  cfloat * restrict ybuf1 =  (cfloat * restrict) outputy1.data();
  save_buff((cfloat * restrict) cbuff , inputx1);
  ssca_cx_multi_0    (  xbuf0, (cfloat * restrict) cbuff,  1024,        ybuf0);
  ssca_cx_multi_0    (  xbuf0 + 4, (cfloat * restrict) cbuff+4,  1024,  ybuf1);

}



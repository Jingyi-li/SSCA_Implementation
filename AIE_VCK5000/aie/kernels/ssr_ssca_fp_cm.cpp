//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_ssca_fp_cm.h"
#include "fft_stages.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_ssca_fp_cm::ssr_ssca_fp_cm()
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_ssca_fp_cm::run( input_buffer_1d<int,2048,0>& __restrict inputx0,
                        input_buffer_1d<int,32,0>& __restrict inputx1,
                        output_buffer_1d<int,1024>& __restrict outputy0,
                        output_buffer_1d<int,1024>& __restrict outputy1 )
{
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  cfloat * restrict cbuff =  (cfloat * restrict) inputx1.data();
  cfloat * restrict ybuf0 =  (cfloat * restrict) outputy0.data();
  cfloat * restrict ybuf1 =  (cfloat * restrict) outputy1.data();

  ssca_cx_multi_0    (  xbuf0, cbuff,  1024,        ybuf0);
  ssca_cx_multi_0    (  xbuf0 + 4, cbuff+4,  1024,  ybuf1);

}



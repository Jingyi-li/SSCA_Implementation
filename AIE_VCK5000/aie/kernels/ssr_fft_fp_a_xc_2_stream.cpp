//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_a_xc_2_stream.h"
#include "fft_stages.h"
#include "ssca_funcs.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_a_xc_2_stream::ssr_fft_fp_a_xc_2_stream(int (&winbuff0_i)[1024], int (&winbuff1_i)[1024], int (&window_coeff_i)[64])
  : winbuff0(winbuff0_i), winbuff1(winbuff1_i), window_coeff(window_coeff_i)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}


inline __attribute__((always_inline)) 
void ssr_fft_fp_a_xc_2_stream::stream_out (cfloat * restrict px0,cfloat * restrict px1, output_window<cfloat>* restrict py0)
{
    // v8float * restrict po1 = (v8float * restrict) py0;
    v8float * restrict pi1 = (v8float * restrict) px0;
    v8float * restrict pi2 = (v8float * restrict) px1;
    v8float coe = upd_elem(upd_elem(undef_v8float(), 0,  1), 1, -1);

    for (int i = 0; i< 2; i++){
        v8float x1 = *pi1++;
        v8float x2 = *pi2++;
        v4cfloat output = as_v4cfloat(fpmul(x1, 0,  0x76543210, coe, 0, 0x10101010));
        for (int j = 0; j< 4; j++){
          window_writeincr(py0, ext_elem(output, j));
          // cfloat a = ext_elem(output,j);
          // printf("conjugate_x[%d] = (%f,%f)\n", i*8+j*2, a.real, a.imag);
        }
        v4cfloat output2 = as_v4cfloat(fpmul(x2, 0,  0x76543210, coe, 0, 0x10101010));
        for (int j = 0; j< 4; j++){
          window_writeincr(py0, ext_elem(output2, j));
          // cfloat a = ext_elem(output2,j);
          // printf("conjugate_x[%d] = (%f,%f)\n", i*8+j*2+1, a.real, a.imag);
        }
    }


}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_a_xc_2_stream::run( input_buffer_1d<int,1024,0>& __restrict inputx0,
                        input_buffer_1d<int,1024,0>& __restrict inputx1,
                        output_buffer_1d<int,2048>& __restrict outputy,
                        output_window<cfloat>* __restrict outputy1  )
{
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  cfloat * restrict xbuf1 =  (cfloat * restrict) inputx1.data();
  cfloat * restrict ybuff =  (cfloat * restrict) outputy.data();
  // cfloat * restrict ybuf1 =  (cfloat * restrict) outputy1.data();

  cfloat * restrict tw = (cfloat * restrict) window_coeff;
  stream_out (xbuf0 + 1024/4,xbuf1 + 1024/4, outputy1);
  // Windoe coeff
  window_chebwin   (  xbuf0, xbuf1, tw, 1024, (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1);
  // Radix-2 stages
  stage0_radix2_dit(  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1,      1024,    ybuff);         // s0
  stage1_radix2_dit(     ybuff,     1024,  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1);  // s1
  stage2_radix2_dit(  (cfloat * restrict) winbuff0, (cfloat * restrict) winbuff1,      1024,    ybuff);         // s2

}



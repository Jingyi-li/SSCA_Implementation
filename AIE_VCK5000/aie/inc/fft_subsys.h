//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __FFT_SUBSYS_H__
#define __FFT_SUBSYS_H__

#include <adf.h>

#include "fft_twiddle.h"
#include "fft_phrom.h"
#include "win_coeff.h"
#include "exp_coeff.h"

#include "ssr_ssca_fp_win.h"
#include "ssr_ssca_fp_dcm.h"
#include "ssr_ssca_fp_cm.h"
#include "ssr_fft_fp_a.h"
#include "ssr_fft_fp_a_2.h"
#include "ssr_fft_fp_a_xc_2.h"
#include "ssr_fft_fp_b.h"
#include "ssr_fft_fp_c.h"
#include "ssr_fft_fp_c_2.h"
#include "ssr_fft_fp_c_dcm.h"
#include "ssr_fft_fp_c_dcm_cd.h"
#include "ssr_fft_fp_d.h"
#include "ssr_fft_fp_e.h"
#include "ssr_fft_fp_rotate.h"
#include "ssr_fft_fp_rotate_2.h"

#include "ssr_fft_fp_a_xc_2_stream.h"
#include "ssr_ssca_fp_cm_stream.h"
#include "ssr_fft_fp_b_stream.h"
#include "ssr_fft_fp_c_dcm_stream.h"

using namespace adf;


class ssr_fft_fp_1kpt_a: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out;

  ssr_fft_fp_1kpt_a(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_a>();
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_a.cpp";

    // Make connections
    connect<>(in[0], core.in[0]);
    connect<>(in[1], core.in[1]);
    connect<>(core.out[0], out);
  };
};



class ssr_win_fft_cx_fp_16x64pt_stream_a: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out[2];

  ssr_win_fft_cx_fp_16x64pt_stream_a(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_a_xc_2_stream>(std::vector<int>(1024),
                                                 std::vector<int>(1024),
                                                 std::vector<int>{CHEBWIN});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_a_xc_2_stream.cpp";

    // Make connections
    connect<>(in[0], core.in[0]);
    connect<>(in[1], core.in[1]);
    connect<>(core.out[0], out[0]);
    connect<window<128>>(core.out[1], out[1]);
    location<stack>(      core)=location<parameter>(core.param[0]);
  };
};


class ssr_cx_fp_16x64pt_stream: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out[2];

  ssr_cx_fp_16x64pt_stream(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_ssca_fp_cm_stream>(std::vector<int>(32));
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_ssca_fp_cm_stream.cpp";

    // Make connections
    connect<>(in[0], core.in[0]);
    connect<window<128>>(in[1], core.in[1]);
    connect<>(core.out[0], out[0]);
    connect<>(core.out[1], out[1]);
  };
};

class ssr_fft_fp_1kpt_b: public graph {
public:
  kernel core;
  port<input>  in;
  port<output> out;

  ssr_fft_fp_1kpt_b(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_b>(std::vector<int>(2048),
                                               std::vector<int>{TWID0},
                                               std::vector<int>{TWID1});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_b.cpp";
    // Make connections
    connect<>(in,  core.in[0]);
    connect<>(core.out[0], out);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};


class ssr_fft_fp_1kpt_b_stream: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out[2];

  ssr_fft_fp_1kpt_b_stream(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_b_stream>(std::vector<int>(2048),
                                               std::vector<int>{TWID0},
                                               std::vector<int>{TWID1});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_b_stream.cpp";
    // Make connections
    connect<>(in[0],  core.in[0]);
    connect<window<128>>(in[1],  core.in[1]);
    connect<>(core.out[0], out[0]);
    connect<window<128>>(core.out[1], out[1]);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};

class ssr_fft_fp_1kpt_c: public graph {
public:
  kernel core;
  port<input>  in;
  port<output> out;

  ssr_fft_fp_1kpt_c(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_c>(std::vector<int>(2048),
                                               std::vector<int>{TWID2},
                                               std::vector<int>{TWID3});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_c.cpp";
    // Make connections
    connect<>(in,  core.in[0]);
    connect<>(core.out[0], out);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};


class ssr_fft_fp_16x64pt_c_dcm_stream: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out[2];

  ssr_fft_fp_16x64pt_c_dcm_stream(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_c_dcm_stream>(std::vector<int>(2048),
                                                   std::vector<int>{TWID2},
                                                   std::vector<int>{DCEXP},
                                                   std::vector<int>(128));
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_c_dcm_stream.cpp";
    // Make connections
    connect<>(in[0],  core.in[0]);
    connect<window<128>>(in[1],  core.in[1]);
    connect<>(core.out[0], out[0]);
    connect<window<128>>(core.out[1], out[1]);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
    location<parameter>(core.param[3])=location<parameter>(core.param[0]);
  };
};


class ssr_fft_fp_1kpt_d: public graph {
public:
  kernel core;
  port<input> in;
  port<output> out;

  ssr_fft_fp_1kpt_d(void) {

        // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_d>(std::vector<int>{TWID4});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_d.cpp";
    // Make connections
    connect<>(in, core.in[0]);
    connect<>(core.out[0], out);
    location<stack>(      core)=location<parameter>(core.param[0]);
  };
};

class ssr_fft_fp_1kpt_e: public graph {
public:
  kernel core;
  port<input> in;
  port<output> out[2];

  ssr_fft_fp_1kpt_e(void) {

    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_e>(std::vector<int>(2048),
                                               std::vector<int>{TWID5},
                                               std::vector<int>{TWID6});

    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_e.cpp";

    // Make connections
    connect<>(in, core.in[0]);
    for(int i=0; i<2; i++) connect<>( core.out[i], out[i]);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};


template<int xoff, int yoff>
class ssr_fft_fp_1kpt_graph: public graph {
public:
  ssr_fft_fp_1kpt_a ka;
  ssr_fft_fp_1kpt_b kb;
  ssr_fft_fp_1kpt_c kc;
  ssr_fft_fp_1kpt_d kd;
  ssr_fft_fp_1kpt_e ke;

  port<input>  in[2];
  port<output> out[2];

  ssr_fft_fp_1kpt_graph(void) {
    connect<>(in[0], ka.in[0]);
    connect<>(in[1], ka.in[1]);

    connect<>(ka.out, kb.in);
    connect<>(kb.out, kc.in);
    connect<>(kc.out, kd.in);
    connect<>(kd.out, ke.in);

    connect<>(ke.out[0], out[0]);
    connect<>(ke.out[1], out[1]);

    // constraints
    location<kernel>(ka.core) = tile(xoff,   yoff);
    location<kernel>(kb.core) = tile(xoff,   yoff+1);
    location<kernel>(kc.core) = tile(xoff+1, yoff);
    location<kernel>(kd.core) = tile(xoff+1, yoff+1);
    location<kernel>(ke.core) = tile(xoff+2, yoff);

    location<buffer>(ka.core.in[0]) = {address(xoff, yoff+1, 0x0000), address(xoff, yoff+1, 0x2000)};
    location<buffer>(ka.core.in[1]) = {address(xoff, yoff+1, 0x1000), address(xoff, yoff+1, 0x3000)};
    location<stack>( ka.core)       =  bank(xoff, yoff+1, 3);

    location<buffer>(kb.core.in[0]) = {address(xoff, yoff,   0x0000), address(xoff, yoff, 0x2000)};
    location<parameter>(kb.core.param[2])=  address(xoff, yoff+1, 0x4000);
    location<stack>(    kb.core)    =  bank(xoff, yoff+1, 3);

    location<buffer>(kc.core.in[0]) = {address(xoff+1, yoff+1,   0x0000), address(xoff+1, yoff+1, 0x2000)};
    location<parameter>(kc.core.param[2])    =  address(xoff,   yoff,     0x4000);
    location<stack>(    kc.core)    =  bank(xoff, yoff, 3);
    //
    location<buffer>(kd.core.in[0]) = {address(xoff+1, yoff+1,   0x4000), address(xoff+1, yoff+1, 0x6000)};
    location<stack>(    kd.core)    =  bank(xoff+1, yoff, 3);
    //
    location<buffer>(ke.core.in[0]) = {address(xoff+1, yoff, 0x0000), address(xoff+1, yoff, 0x2000)};
    location<parameter>(ke.core.param[2])    =  address(xoff+1, yoff, 0x4000);
    location<stack>(    ke.core)    =  bank(   xoff+2, yoff+1, 2);
    location<buffer>(ke.core.out[0]) = {address(xoff+2, yoff+1,   0x0000), address(xoff+2, yoff+1, 0x2000)};
    location<buffer>(ke.core.out[1]) = {address(xoff+2, yoff+1,   0x1000), address(xoff+2, yoff+1, 0x3000)};

  };
};


// custom added: window+fft+dc+cm stream
template<int xoff, int yoff>
class ssr_ssca_win_fft_dc_cm_stream2_fp_64pt_x16_graph: public graph {
public:
  ssr_win_fft_cx_fp_16x64pt_stream_a ka;
  ssr_fft_fp_1kpt_b_stream kb;
  ssr_fft_fp_16x64pt_c_dcm_stream kc;
  ssr_cx_fp_16x64pt_stream kd;

  port<input>  in[2];
  port<output> out[2];

  ssr_ssca_win_fft_dc_cm_stream2_fp_64pt_x16_graph(void) {
    connect<>(in[0], ka.in[0]);
    connect<>(in[1], ka.in[1]);

    connect<>(ka.out[0], kb.in[0]);
    connect<window<128>>(ka.out[1], kb.in[1]);
    connect<>(kb.out[0], kc.in[0]);
    connect<window<128>>(kb.out[1], kc.in[1]);
    connect<>(kc.out[0], kd.in[0]);
    connect<window<128>>(kc.out[1], kd.in[1]);
    connect<>(kd.out[0],out[0]);
    connect<>(kd.out[1],out[1]);


    // constraints
    location<kernel>(ka.core) = tile(xoff,   yoff);
    location<kernel>(kb.core) = tile(xoff,   yoff+1);
    location<kernel>(kc.core) = tile(xoff+1, yoff+1);
    location<kernel>(kd.core) = tile(xoff+1, yoff);

    location<buffer>(ka.core.in[0]) = {address(xoff, yoff+1, 0x0000), address(xoff, yoff+1, 0x2000)};
    location<buffer>(ka.core.in[1]) = {address(xoff, yoff+1, 0x1000), address(xoff, yoff+1, 0x3000)};
    location<parameter>(ka.core.param[1])    =  address(xoff, yoff, 0x4000);
    location<parameter>(ka.core.param[2])    =  address(xoff, yoff, 0x6000);
    location<stack>( ka.core)       =  bank(xoff, yoff+1, 3);

    location<buffer>(kb.core.in[0]) = {address(xoff, yoff,   0x0000), address(xoff, yoff, 0x2000)};
    location<parameter>(kb.core.param[2])=  address(xoff, yoff+1, 0x4000);
    location<stack>(    kb.core)    =  bank(xoff, yoff+1, 3);

    location<buffer>(kc.core.in[0]) = {address(xoff+1, yoff+1,   0x0000), address(xoff+1, yoff+1, 0x2000)};
    location<parameter>(kc.core.param[2])    =  address(xoff+2,   yoff+1,     0x4000);
    location<stack>(    kc.core)    =  bank(xoff+1, yoff, 3);
    //
    location<buffer>(kd.core.in[0]) = {address(xoff+1, yoff+1,   0x4000), address(xoff+1, yoff+1, 0x6000)};
    location<stack>(    kd.core)    =  bank(xoff, yoff, 3);
    location<buffer>(kd.core.out[0]) = {address(xoff+1, yoff,   0x0000), address(xoff+1, yoff, 0x2000)};
    location<buffer>(kd.core.out[1]) = {address(xoff+1, yoff,   0x1000), address(xoff+1, yoff, 0x3000)};
  };
};

#endif //__FFT_SUBSYS_H__

// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.h"
#include "rotate_coeff.h"

// storage size [1024][72]; window_id[idx*LANE+0:idx*LANE+LANE-1]
void loadA(ap_uint<DWIDTH_64> a_buf[M][Np+LANE], ap_uint<AXI_WIDTH_A>* memin, const int idx){
#pragma HLS inline off 
    for(int ic = 0; ic<M; ic++){
        for(int ir = 0; ir<Np/LANE+1; ir++){
        #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata = memin[idx + ic*(M/LANE) + ir];
            for(int il = 0; il<LANE; il++){
            #pragma HLS unroll
                a_buf[ic][ir*LANE+il] = tempdata(63 + il * 64, 0 + il*64);
            }
        }
    }
}

// pass the data to stream a:0/2/4/6/8/10/12/14 and stream b:1/3/5/7/9/11/13/15; idx is the iteration of 16x64pt; xoff is the xshift_offset windowid[xoff] xoff = [0:LANE-1]
void sendA(ap_uint<DWIDTH_64> a_buff[M][Np+LANE], axis_data_64 &stream_a, axis_data_64 &stream_b, const int idx, const int xoff){
#pragma HLS inline off
    for(int ic = 0; ic < Np; ic++){
        for(int i = 0; i < 16; i+=2){
        #pragma HLS PIPELINE II=1
            data_64 xa, xb;
            xa.data = a_buff[idx*16+i  ][ic+xoff];
            xb.data = a_buff[idx*16+i+1][ic+xoff];
            stream_a.write(xa);
            stream_b.write(xb);
        }
    }
}

void input_A(
    axis_data_64 &fft_in_00a, axis_data_64 &fft_in_00b,
    ap_uint<AXI_WIDTH_A>* memin, const int iter
){
#pragma HLS inline off 
    ap_uint<DWIDTH_64> buff0_A[2][M][Np+LANE];
    #pragma HLS bind_storage variable=buff0_A type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_A type=cyclic factor=LANE dim=3

    // #pragma HLS PIPELINE II = 1
    loadA(buff0_A[0], memin, 0);
    for(int it = 1; it< iter/LANE; it++){ // mem++ have LANE values
        loadA(buff0_A[it%2], memin, it);
        for(int ishift = 0; ishift<LANE; ishift++){ // xoff
            for(int im = 0; im<M/16/KN; im++){ // iterations
                sendA(buff0_A[(it-1)%2], fft_in_00a, fft_in_00b, im, ishift);
            }
        }   
    }
    for(int ishift = 0; ishift<LANE; ishift++){
        for(int im = 0; im<M/16/KN; im++){
            sendA(buff0_A[(iter/LANE-1)%2], fft_in_00a, fft_in_00b, im, ishift);
        }
    }   
}


void loadB(ap_uint<DWIDTH_64> b_buf[Np][M/2], axis_data_64 &fft_mid_out)
{
#pragma HLS inline off 
    for(int im = 0; im<M/2/8/KN; im++){
        for(int ip = 0; ip<Np;ip++){
            for(int il = 0; il < 8; il++){
            #pragma HLS PIPELINE II = 1
                data_64 x1 = fft_mid_out.read();
                b_buf[ip][im*8+il] = x1.data;
            }
        }
    }
}


void sendB(ap_uint<DWIDTH_64> b_buf[Np][M/2], axis_data_64 &fft_mid_in)
{
#pragma HLS inline off 
    for(int ip = 0; ip<Np/KN;ip++){
        for(int il = 0; il < M/2; il++){
        #pragma HLS PIPELINE II = 1
            data_64 x1;
            x1.data = b_buf[ip][il];
            fft_mid_in.write(x1);
        }
    }
}

void sendBexp(axis_data_64 &fft_rotate_coeff, const int iter)
{
#pragma HLS inline off
    for(int idx = 0; idx<iter; idx++){
        int startidx = idx*48;
        for(int ip = 0; ip<Np/KN; ip++){
            for(int il = 0; il<48; il+=2){
            #pragma HLS PIPELINE II = 1
                data_64 x;
                x.data(31,0) = RTEXP[startidx+il];
                x.data(63,32) = RTEXP[startidx+il+1];
                fft_rotate_coeff.write(x);
            }
        }
    }
}

void transpose(
    axis_data_64 &fft_mid_out_00a, axis_data_64 &fft_mid_out_00b,
    axis_data_64 &fft_mid_in_00a, axis_data_64 &fft_mid_in_00b, const int iter
){
#pragma HLS inline off 
    ap_uint<DWIDTH_64> buff00a[2][Np][M/2];
    ap_uint<DWIDTH_64> buff00b[2][Np][M/2];
    #pragma HLS bind_storage variable=buff00a type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff00a complete dim=1
    #pragma HLS bind_storage variable=buff00b type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff00b complete dim=1

    Init:
    {
        loadB(buff00a[0], fft_mid_out_00a);
        loadB(buff00b[0], fft_mid_out_00b);
    }

    for(int idx = 1; idx < iter; idx++){
        loadB(buff00a[idx%2],       fft_mid_out_00a);
        loadB(buff00b[idx%2],       fft_mid_out_00b);
        sendB(buff00a[(idx-1)%2],    fft_mid_in_00a);
        sendB(buff00b[(idx-1)%2],    fft_mid_in_00b);
    }
    Final:
    {
        sendB(buff00a[(iter-1)%2],    fft_mid_in_00a);
        sendB(buff00b[(iter-1)%2],    fft_mid_in_00b);
    }
    
}
void storeB(ap_uint<DWIDTH_64> b_bufa[Np][M/2], ap_uint<DWIDTH_64> b_bufb[Np][M/2], ap_uint<AXI_WIDTH_A>* memout, const int idx)
{
#pragma HLS inline off
    for (int ic = 0; ic< M/2; ic++){
        for(int ir = 0; ir<Np/LANE;ir++){
            #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata;
            for(int il = 0; il<LANE; il++){
            #pragma HLS unroll
                tempdata(63 + il * 64, 0 + il*64) = b_bufa[ir*LANE+il][ic];
            }
            memout[ir+ic*(Np/LANE)*2+idx*(Np/LANE)*M] = tempdata;
        }
        for(int ir = 0; ir<Np/LANE;ir++){
            #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata;
            for(int il = 0; il<LANE; il++){
            #pragma HLS unroll
                tempdata(63 + il * 64, 0 + il*64) = b_bufb[ir*LANE+il][ic];
            }
            memout[ir+(ic*2+1)*(Np/LANE)+idx*(Np/LANE)*M] = tempdata;
        }
    }
}

void Output_A(
    axis_data_64 &fft_mid_out_00a, axis_data_64 &fft_mid_out_00b,
    ap_uint<AXI_WIDTH_A>* memout, const int iter
){
#pragma HLS inline off 
    ap_uint<DWIDTH_64> buff00a[2][Np][M/2];
    ap_uint<DWIDTH_64> buff00b[2][Np][M/2];
    #pragma HLS bind_storage variable=buff00a type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff00a complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff00a type=cyclic factor=LANE dim=2
    #pragma HLS bind_storage variable=buff00b type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff00b complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff00b type=cyclic factor=LANE dim=2

    Init:
    {
        loadB(buff00a[0], fft_mid_out_00a);
        loadB(buff00b[0], fft_mid_out_00b);
    }

    for(int idx = 1; idx < iter; idx++){
        loadB(buff00a[idx%2],       fft_mid_out_00a);
        loadB(buff00b[idx%2],       fft_mid_out_00b);
        storeB(buff00a[(idx-1)%2], buff00b[(idx-1)%2], memout, idx-1);
    }
    Final:
    {
        storeB(buff00a[(iter-1)%2], buff00b[(iter-1)%2], memout, iter-1);
    }
    
}


void storeC(ap_uint<DWIDTH_64> c_buf[M][Np], ap_uint<AXI_WIDTH_A>* memout, const int idx){
#pragma HLS inline off 
    for(int ic = 0; ic<M; ic++){
        storeC_for_Np:
        for(int ir = 0; ir<Np/LANE; ir++){
            #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata;
            for(int il = 0; il<LANE; il++){
            #pragma HLS unroll
                tempdata(63 + il * 64, 0 + il*64) = c_buf[ic][ir*LANE+il];
            }
            // memout[ic*(Np/LANE)*M+ir+idx*(Np/LANE)] = tempdata;
            memout[ic*(Np/LANE)+ir+idx*(Np/LANE)*M] = tempdata;
        }
    }
}

// void storeC(ap_uint<DWIDTH_64> c_buf[M][Np], ap_uint<AXI_WIDTH_A>* memout, const int idx){
// #pragma HLS inline off 
//     storeC_for_Np:
//     for(int ir = 0; ir<Np/LANE; ir++){
//         storeC_for_M:
//         for(int ic = 0; ic<M; ic++){
//         #pragma HLS PIPELINE II=1
//             ap_uint<AXI_WIDTH_A> tempdata;
//             for(int il = 0; il<LANE; il++){
//             #pragma HLS unroll
//                 tempdata(63 + il * 64, 0 + il*64) = c_buf[ic][ir*LANE+il];
//             }
//             // memout[ic*(Np/LANE)*M+ir+idx*(Np/LANE)] = tempdata;
//             memout[ic*M+ir*M*M+idx] = tempdata;
//         }
//     }
// }

void recieveC(ap_uint<DWIDTH_64> c_buf[M][Np],axis_data_64 &fft1_out_00a, axis_data_64 &fft1_out_00b)
{
#pragma HLS inline off
    recieveC_for_Np:
    for (int i = 0; i< Np/KN; i++){
        recieveC_for_M:
        for (int im = 0; im < M/8; im++){
            for (int j = 0; j<4; j++){
                #pragma HLS PIPELINE II=1
                data_64 a = fft1_out_00a.read();
                c_buf[im*8+j][i] = a.data;
                data_64 b = fft1_out_00b.read();
                c_buf[im*8+j+4][i] = b.data;
            }
        }
    }
}


void output_C(
    axis_data_64 &fft1_out_00a, axis_data_64 &fft1_out_00b,
    ap_uint<AXI_WIDTH_A>* memout,const int iter
){
#pragma HLS inline off 
    ap_uint<DWIDTH_64> buff0_C[2][M][Np];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_C type=cyclic factor=LANE dim=3

    // #pragma HLS PIPELINE II = 1
    recieveC(buff0_C[0], fft1_out_00a, fft1_out_00b);
    for(int it = 1; it< iter; it++){
        recieveC(buff0_C[it%2], fft1_out_00a, fft1_out_00b);
        storeC(buff0_C[(it-1)%2], memout, it-1);
    }
    storeC(buff0_C[1], memout, iter-1);
}


void loadD(ap_uint<DWIDTH_64> d_buf[Np*Tl][M], ap_uint<AXI_WIDTH_A>* memin, const int idx){
#pragma HLS inline off 
    loadD_for_M:
    for(int ic = 0; ic<M; ic++){
        loadD_for_Np:
        for(int ir = 0; ir<Np*Tl/LANE; ir++){
        #pragma HLS PIPELINE II=1
            // ap_uint<AXI_WIDTH_A> tempdata = memin[idx*M*(Np/LANE) + ic*(Np/LANE) + ir];
            ap_uint<AXI_WIDTH_A> tempdata = memin[idx*(Np*Tl/LANE) + ic*(Np/LANE)*M + ir];
            for(int il = 0; il<LANE; il++){
            #pragma HLS unroll
                d_buf[ir*LANE+il][ic] = tempdata(63 + il * 64, 0 + il*64);
            }
        }
    }
}

// void loadD(ap_uint<DWIDTH_64> d_buf[Np][M], ap_uint<AXI_WIDTH_A>* memin, const int idx){
// #pragma HLS inline off 
//     loadD_for_Np:
//     for(int ir = 0; ir<Np/LANE; ir++){
//         loadD_for_M:
//         for(int ic = 0; ic<M; ic++){
//         #pragma HLS PIPELINE II=1
//             ap_uint<AXI_WIDTH_A> tempdata = memin[idx*M + ic + ir*M*M];
//             for(int il = 0; il<LANE; il++){
//             #pragma HLS unroll
//                 d_buf[ir*LANE+il][ic] = tempdata(63 + il * 64, 0 + il*64);
//             }
//         }
//     }
// }

void sendD(ap_uint<DWIDTH_64> d_buff[Np*Tl][M], axis_data_64 &stream_a, axis_data_64 &stream_b){
#pragma HLS inline off
    sendD_for_Np:
    for(int ic = 0; ic < Np*Tl/KN; ic++){
        sendD_for_M:
        for(int i = 0; i < M; i+=2){
        #pragma HLS PIPELINE II=1
            data_64 xa, xb;
            xa.data = d_buff[ic][   i];
            xb.data = d_buff[ic][ i+1];
            stream_a.write(xa);
            stream_b.write(xb);
        }
    }
}

void input_D(
    axis_data_64 &fft2_in_00a, axis_data_64 &fft2_in_00b,
    ap_uint<AXI_WIDTH_A>* memintm, const int iter
){
#pragma HLS inline off 
    ap_uint<DWIDTH_64> buff0_D[2][Np*Tl][M];
    #pragma HLS bind_storage variable=buff0_D type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_D complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_D type=cyclic factor=LANE dim=2

    // #pragma HLS PIPELINE II = 1
    loadD(buff0_D[0], memintm, 0);
    for(int it = 1; it< iter/Tl; it++){
        loadD(buff0_D[it%2], memintm, it);
        sendD(buff0_D[(it-1)%2], fft2_in_00a, fft2_in_00b);
    }
    sendD(buff0_D[(iter/Tl-1)%2], fft2_in_00a, fft2_in_00b);
}


void transpose_B(
    axis_data_64 &scd_out_00a, axis_data_64 &scd_out_00b,
    axis_data_64 &fft1_in_00a, axis_data_64 &fft1_in_00b, 
    axis_data_64 &fft1_in_00c,
    const int iter
){
    transpose(scd_out_00a, scd_out_00b, fft1_in_00a, fft1_in_00b, iter);
    sendBexp(fft1_in_00c, iter);
}

void cdp_fft1(
    axis_data_64 &scd_in_00a, axis_data_64 &scd_in_00b,
    axis_data_64 &scd_out_00a, axis_data_64 &scd_out_00b,
    axis_data_64 &fft1_in_00a, axis_data_64 &fft1_in_00b, 
    axis_data_64 &fft1_in_00c,
    axis_data_64 &fft1_out_00a, axis_data_64 &fft1_out_00b,
    ap_uint<AXI_WIDTH_A> *memin, ap_uint<AXI_WIDTH_A> *memintm,
    const int iter
){
    input_A(scd_in_00a, scd_in_00b, memin, iter);
    transpose_B(scd_out_00a, scd_out_00b, fft1_in_00a, fft1_in_00b, fft1_in_00c, iter);
    output_C(fft1_out_00a, fft1_out_00b, memintm, iter);
}


void input_test(
    axis_data_64 &fft_in_00a, axis_data_64 &fft_in_00b,
    ap_uint<AXI_WIDTH_A>* memin, const int iter
){
#pragma HLS inline off 
    for(int it = 0; it< iter; it++){ 
        for(int im = 0; im<M/LANE; im++){ // xoff
        #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata = memin[it*(M/LANE)+im];
            for(int il = 0; il<LANE; il+=2){ // iterations
                data_64 xa, xb;
                xa.data = tempdata(63 + il * 64, 0 + il*64);
                xb.data = tempdata(63 + il * 64+64, 0 + il*64+64);
                fft_in_00a.write(xa);
                fft_in_00b.write(xb);
            }
        }   
    }
}

void input_test_coeff(
    axis_data_64 &fft_rotate_coeff, const int iter
){
#pragma HLS inline off 
   for(int idx = 0; idx<iter; idx++){
        int startidx = idx*48;
        for(int il = 0; il<48; il+=2){
        #pragma HLS PIPELINE II = 1
            data_64 x;
            x.data(31,0) = RTEXP[startidx+il];
            x.data(63,32) = RTEXP[startidx+il+1];
            fft_rotate_coeff.write(x);
        }
    }
}

void output_test(
    axis_data_64 &fft_out_00a, axis_data_64 &fft_out_00b,
    ap_uint<AXI_WIDTH_A>* memout, const int iter
){
#pragma HLS inline off 
    for(int it = 0; it< iter; it++){ 
        for(int im = 0; im<M/LANE; im++){
            #pragma HLS PIPELINE II=1
            ap_uint<AXI_WIDTH_A> tempdata;
            for(int il = 0; il<LANE; il+=2){ 
                data_64 xa = fft_out_00a.read();
                data_64 xb = fft_out_00b.read();
                tempdata(63 + il * 64, 0 + il*64) = xa.data;
                tempdata(63 + il * 64+64, 0 + il*64+64) = xb.data;
            }
            memout[it*(M/LANE)+im] = tempdata;
        }   
    }
}

void dma_hls(
    // ap_uint<AXI_WIDTH_A> *memin, ap_uint<AXI_WIDTH_A> *memout, 
    ap_uint<AXI_WIDTH_A> *memin, ap_uint<AXI_WIDTH_A> *memintm, ap_uint<AXI_WIDTH_A> *memout, 
    axis_data_64 &scd_in_00a, axis_data_64 &scd_in_00b,
    axis_data_64 &scd_out_00a, axis_data_64 &scd_out_00b,
    // const int iter
    axis_data_64 &fft1_in_00a, axis_data_64 &fft1_in_00b, axis_data_64 &fft1_in_00c,
    axis_data_64 &fft1_out_00a, axis_data_64 &fft1_out_00b,
    axis_data_64 &fft2_in_00a, axis_data_64 &fft2_in_00b,
    axis_data_64 &fft2_out_00a, axis_data_64 &fft2_out_00b,
    const int iter
    )
{
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=memin max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=memin
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=memintm max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=memintm
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=memout max_write_burst_length=16 num_write_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=memout
    #pragma HLS interface s_axilite bundle=control port=iter
    // #pragma HLS interface s_axilite bundle=control port=iter2
    
    #pragma HLS INTERFACE axis port=scd_in_00a
    #pragma HLS INTERFACE axis port=scd_in_00b
    #pragma HLS INTERFACE axis port=scd_out_00a
    #pragma HLS INTERFACE axis port=scd_out_00b
    #pragma HLS INTERFACE axis port=fft1_in_00a
    #pragma HLS INTERFACE axis port=fft1_in_00b
    #pragma HLS INTERFACE axis port=fft1_in_00c
    #pragma HLS INTERFACE axis port=fft1_out_00a
    #pragma HLS INTERFACE axis port=fft1_out_00b
    #pragma HLS INTERFACE axis port=fft2_in_00a
    #pragma HLS INTERFACE axis port=fft2_in_00b
    #pragma HLS INTERFACE axis port=fft2_out_00a
    #pragma HLS INTERFACE axis port=fft2_out_00b

    #pragma HLS INTERFACE s_axilite port=return bundle=control  

    #pragma HLS DATAFLOW

    

    cdp_fft1(
    scd_in_00a, scd_in_00b,
    scd_out_00a, scd_out_00b,
    fft1_in_00a, fft1_in_00b, fft1_in_00c,
    fft1_out_00a, fft1_out_00b,
    memin, memintm,iter);
    input_D(fft2_in_00a, fft2_in_00b, memintm,iter);
    output_C(fft2_out_00a, fft2_out_00b, memout,iter);

    // input_A(scd_in_00a, scd_in_00b, memin, iter);
    // Output_A(scd_out_00a, scd_out_00b, memout, iter);

    // input_A(scd_in_00a, scd_in_00b, memin, iter);
    // transpose_B(scd_out_00a, scd_out_00b, fft1_in_00a, fft1_in_00b, fft1_in_00c, iter);
    // output_C(fft1_out_00a, fft1_out_00b, memout, iter);
}

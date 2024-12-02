//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __SSCA_FUNCS_H__
#define __SSCA_FUNCS_H__
#include <adf.h>

inline __attribute__((always_inline)) void window_chebwin (cfloat * restrict px0,  cfloat * restrict px1, cfloat * restrict tw, unsigned int n, cfloat * restrict py0, cfloat * restrict py1)
{
    v8float * restrict po1 = (v8float * restrict) py0;
    v8float * restrict po2 = (v8float * restrict) (py0 + n/4);
    v8float * restrict po3 = (v8float * restrict) py1;
    v8float * restrict po4 = (v8float * restrict) (py1 + n/4);

    v8float * restrict pi1 = (v8float * restrict) px0;
    v8float * restrict pi2 = (v8float * restrict) (px0 + n/4);
    v8float * restrict pi3 = (v8float * restrict) px1;
    v8float * restrict pi4 = (v8float * restrict) (px1 + n/4);

    v8float * restrict pc =  ( v8float * restrict) tw;
    v8float * restrict pc1 =  ( v8float * restrict) (tw + 32/2);

    v8float coe = *pc++;
    v8float coe1 = *pc1++;
    
    for (int j = 0; j < (n/2/16/8); j++)
    {
        
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 0, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 0, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 0, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 0, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 0, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 0, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 0, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 0, 0);// 521/523/525/527//

        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 1, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 1, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 1, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 1, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 1, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 1, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 1, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 1, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 2, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 2, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 2, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 2, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 2, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 2, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 2, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 2, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 3, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 3, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 3, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 3, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 3, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 3, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 3, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 3, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 4, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 4, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 4, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 4, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 4, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 4, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 4, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 4, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 5, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 5, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 5, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 5, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 5, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 5, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 5, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 5, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 6, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 6, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 6, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 6, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 6, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 6, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 6, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 6, 0);// 521/523/525/527//
        }
        {
            v8float x1 = *pi1++;
            v8float x2 = *pi2++;
            *po1++ = fpmul(x1, 0, 0x76543210, coe, 7, 0); // 0/2/4/6//16/18/20/22
            *po2++ = fpmul(x2, 0, 0x76543210, coe1, 7, 0);// 512/514/516/518//
            v8float x3 = *pi1++;
            v8float x4 = *pi2++;
            *po1++ = fpmul(x3, 0, 0x76543210, coe, 7, 0);// 8/10/12/14//24/26/28/30
            *po2++ = fpmul(x4, 0, 0x76543210, coe1, 7, 0);// 520/522/524/526//

            v8float x5 = *pi3++;
            v8float x6 = *pi4++;
            *po3++ = fpmul(x5, 0, 0x76543210, coe, 7, 0);// 1/3/5/7//
            *po4++ = fpmul(x6, 0, 0x76543210, coe1, 7, 0);// 513/515/517/519//
            v8float x7 = *pi3++;
            v8float x8 = *pi4++;
            *po3++ = fpmul(x7, 0, 0x76543210, coe, 7, 0);// 9/11/13/15//
            *po4++ = fpmul(x8, 0, 0x76543210, coe1, 7, 0);// 521/523/525/527//
        }
        coe = *pc++;
        coe1 = *pc1++;
    }
    
}

inline __attribute__((always_inline)) void ssca_cx (cfloat * restrict px0, cfloat * restrict py0)
{
    v8float * restrict po1 = (v8float * restrict) py0;
    v8float * restrict pi1 = (v8float * restrict) px0;
    v8float coe = upd_elem(upd_elem(undef_v8float(), 0,  1), 1, -1);

    for (int i = 0; i< 4; i++){
        v8float x1 = *pi1++;
        *po1++ = fpmul(x1, 0,  0x76543210, coe, 0, 0x10101010);
    }


}

inline __attribute__((always_inline)) void ssca_cx_stream (cfloat * restrict px0, output_stream_accfloat * restrict py0)
{
    // v8float * restrict po1 = (v8float * restrict) py0;
    v8float * restrict pi1 = (v8float * restrict) px0;
    v8float coe = upd_elem(upd_elem(undef_v8float(), 0,  1), 1, -1);

    for (int i = 0; i< 4; i++){
        v8float x1 = *pi1++;
        v8float output = fpmul(x1, 0,  0x76543210, coe, 0, 0x10101010);
        writeincr_v8(py0, output);
    }


}

inline __attribute__((always_inline)) void ssca_dc_multi (cfloat * restrict px0, cfloat * restrict tw, unsigned int n, cfloat * restrict py0)
{
    v4cfloat * restrict po1 = (v4cfloat * restrict) py0;
    v4cfloat * restrict po2 = (v4cfloat * restrict) (py0 + n/2);

    v4cfloat * restrict pi1 = (v4cfloat * restrict) px0;
    v4cfloat * restrict pi2 = (v4cfloat * restrict) (px0 + n/2);

    cfloat * restrict pc1 = (cfloat * restrict) tw;
    cfloat * restrict pc2 = (cfloat * restrict) (tw + 64/2);


    {
        for (int i = 0; i<(n/2/4/4); i++){
            v8float coe1 = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc1++));
            v8float coe2 = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc2++));
            for (int j = 0; j<4; j++){
                v4cfloat x1 = *pi1++;
                v4cfloat x2 = *pi2++;
                v4cfloat y1 = fpmul(x1, 0,  0x76543210, coe1, 0, 0);
                v4cfloat y2 = fpmul(x2, 0,  0x76543210, coe2, 0, 0);
                *po1++ = fpmac_conf(y1, x1, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                *po2++ = fpmac_conf(y2, x2, 0,  0x67452301, coe2, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
            }
        }
    }
}

inline __attribute__((always_inline)) void ssca_dc_multi_0 (cfloat * restrict px0, cfloat * restrict tw, cfloat * restrict cx, unsigned int n, cfloat * restrict py0)
{
    v4cfloat * restrict po1 = (v4cfloat * restrict) py0;
    v4cfloat * restrict po2 = (v4cfloat * restrict) (py0 + n/4);

    v4cfloat * restrict pi1 = (v4cfloat * restrict) px0;
    v4cfloat * restrict pi2 = (v4cfloat * restrict) (px0 + n/2);

    cfloat * restrict pc1 = (cfloat * restrict) tw;
    v8float px1 = *((v8float * restrict) cx);
    v8float px2 = *((v8float * restrict) (cx + 8));



    {
        for (int i = 0; i<(n/2/4/4); i++){
            v8float coe1 = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc1++));
            {//0/2/4/6
                v4cfloat x1 = *pi1;
                v4cfloat x2 = *pi2;
                pi1 +=2; 
                pi2 +=2;
                v4cfloat y1 = fpmul(x1, 0,  0x76543210, coe1, 0, 0);
                v4cfloat y2 = fpmul(x2, 0,  0x76543210, coe1, 0, 0);
                // *po1++ = fpmac_conf(y1, x1, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                // *po2++ = fpmac_conf(y2, x2, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat t1 = fpmac_conf(y1, x1, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat t2 = fpmac_conf(y2, x2, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat m1 = fpmul(t1, 0,  0x76543210, px1, 0, 0x6420);
                v4cfloat m2 = fpmul(t2, 0,  0x76543210, px1, 0, 0x6420);
                *po1++      = fpmac_conf(m1, t1, 0,  0x67452301, px1, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                *po2++      = fpmac_conf(m2, t2, 0,  0x67452301, px1, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
            }
            {//8/10/12/14

                v4cfloat x1 = *pi1;
                v4cfloat x2 = *pi2;
                pi1 +=2; 
                pi2 +=2;
                v4cfloat y1 = fpmul(x1, 0,  0x76543210, coe1, 0, 0);
                v4cfloat y2 = fpmul(x2, 0,  0x76543210, coe1, 0, 0);
                // *po1++ = fpmac_conf(y1, x1, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                // *po2++ = fpmac_conf(y2, x2, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat t1 = fpmac_conf(y1, x1, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat t2 = fpmac_conf(y2, x2, 0,  0x67452301, coe1, 1, 0, false, false, fpadd_add, 0x55, fpcmp_nrm);
                v4cfloat m1 = fpmul(t1, 0,  0x76543210, px2, 0, 0x6420);
                v4cfloat m2 = fpmul(t2, 0,  0x76543210, px2, 0, 0x6420);
                *po1++      = fpmac_conf(m1, t1, 0,  0x67452301, px2, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                *po2++      = fpmac_conf(m2, t2, 0,  0x67452301, px2, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
            }
            

        }
    }
}

inline __attribute__((always_inline)) void ssca_cx_multi_0 (cfloat * restrict px0, cfloat * restrict cx, unsigned int n, cfloat * restrict py0)
{
    v4cfloat * restrict po1 = (v4cfloat * restrict) py0;
    v4cfloat * restrict po2 = (v4cfloat * restrict) (py0 + n/4);

    v4cfloat * restrict pi1 = (v4cfloat * restrict) px0;
    v4cfloat * restrict pi2 = (v4cfloat * restrict) (px0 + n/2);

    v8float px1 = *((v8float * restrict) cx);
    v8float px2 = *((v8float * restrict) (cx + 8));



    {
        for (int i = 0; i<(n/2/4/4); i++){
            {//0/2/4/6
                v4cfloat x1 = *pi1;
                v4cfloat x2 = *pi2;
                pi1 +=2; 
                pi2 +=2;
                v4cfloat m1 = fpmul(x1, 0,  0x76543210, px1, 0, 0x6420);
                v4cfloat m2 = fpmul(x2, 0,  0x76543210, px1, 0, 0x6420);
                *po1++      = fpmac_conf(m1, x1, 0,  0x67452301, px1, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                *po2++      = fpmac_conf(m2, x2, 0,  0x67452301, px1, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                // *po1++ = x1;
                // *po2++ = x2;
            }
            {//8/10/12/14

                v4cfloat x1 = *pi1;
                v4cfloat x2 = *pi2;
                pi1 +=2; 
                pi2 +=2;
                v4cfloat m1 = fpmul(x1, 0,  0x76543210, px2, 0, 0x6420);
                v4cfloat m2 = fpmul(x2, 0,  0x76543210, px2, 0, 0x6420);
                *po1++      = fpmac_conf(m1, x1, 0,  0x67452301, px2, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                *po2++      = fpmac_conf(m2, x2, 0,  0x67452301, px2, 1, 0x66442200, false, false, fpadd_add, 0x55, fpcmp_nrm);
                // *po1++ = x1;
                // *po2++ = x2;
            }
            

        }
    }
}

inline __attribute__((always_inline)) void ssca_cx_multi (cfloat * restrict px0, cfloat * restrict py0)
{
    v8float * restrict po1 = (v8float * restrict) py0;
    v8float * restrict pi1 = (v8float * restrict) px0;

    for (int i = 0; i< 4; i++){
        v8float x1 = *pi1++;
        *po1++ = x1;
    }


}
#endif // __SSCA_FUNCS_H__


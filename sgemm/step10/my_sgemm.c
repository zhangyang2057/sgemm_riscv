/*
 * --------------------------------------------------------------------------
 * BLISLAB
 * --------------------------------------------------------------------------
 * Copyright (C) 2016, The University of Texas at Austin
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of The University of Texas nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * bl_sgemm.c
 *
 *
 * Purpose:
 * this is the main file of blislab sgemm.
 *
 * Todo:
 *
 *
 * Modification:
 *
 *
 * */


#include <bl_sgemm.h>
#if __riscv_vector
#include <riscv_vector.h>
#endif

#if 0
void bl_sgemm(
    int    M,
    int    N,
    int    K,
    float *A,
    int    lda,
    float *B,
    int    ldb,
    float *C,        // must be aligned
    int    ldc        // ldc must also be aligned
)
{
    size_t vlmax = vsetvlmax_e32m1();

    for (int32_t m = 0; m < M; m++) {
        for (int32_t n = 0; n < N; n++) {
            const float *ptr_a = &A[m * K];
            const float *ptr_b = &B[n];
            vfloat32m1_t vec_s = vfmv_v_f_f32m1(0.f, vlmax);
            vfloat32m1_t vec_zero = vfmv_v_f_f32m1(0.f, vlmax);
            int k = K;
            for (size_t vl = 0; k > 0; k -= vl) {
                vl = vsetvl_e32m1(k);
                vfloat32m1_t vec_a = vle32_v_f32m1(ptr_a, vl);
                vfloat32m1_t vec_b = vlse32_v_f32m1(ptr_b, N * sizeof(float), vl);
                ptr_a += vl;
                ptr_b += vl * N;
                vec_s = vfmacc_vv_f32m1(vec_s, vec_a, vec_b, vl);
            }

            vfloat32m1_t vec_sum;
            vec_sum = vfredosum_vs_f32m1_f32m1(vec_sum, vec_s, vec_zero, vlmax);
            C[m * N + n]= vfmv_f_s_f32m1_f32(vec_sum);
        }
    }
}

#else
void bl_sgemm(
    int    M,
    int    N,
    int    K,
    float *A,
    int    lda,
    float *B,
    int    ldb,
    float *C,        // must be aligned
    int    ldc        // ldc must also be aligned
)
{
    size_t vl;
    for (int k = 0; k < K; k++) {
        for (int m = 0; m < M; m++) {
            float a = A[m*K + k];
            for (int n = 0; n < N; n += vl) {
                vl = vsetvl_e32m1(N - n);
                vfloat32m1_t b = vle32_v_f32m1(&B[k*N + n], vl);
                vfloat32m1_t c = vle32_v_f32m1(&C[m*N + n], vl);
                c = vfmacc_vf_f32m1(c, a, b, vl);
                vse32_v_f32m1(&C[m*N + n], c, vl);
            }
        }
    }
}

#endif

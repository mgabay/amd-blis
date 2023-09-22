/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name(s) of the copyright holder(s) nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef LPGEMM_FUNC_MAP_H
#define LPGEMM_FUNC_MAP_H

// The XMACRO follows the format ID,FUNC_PTR:
// ID = One of the AOCL_OPERATION_TYPE enum.
// FUNC_PTR = Kernel associated with the AOCL_OPERATION_TYPE.
// It is to be noted that the main macros are defined for combinations
// of ISA types, and in case a kernel is not implemented for a particualr
// ISA combination, the reference kernel should be set as FUNC_PTR.
// TODO: Add reference kernels for BF16/VNNI kernels for ISA combinations
// that is not supported.

// Genoa
#define LPGEMM_KERN_FUNC_MAP_AVX512_VNNI_BF16 \
	KMACRO(U8S8S16OS16, lpgemm_rowvar_u8s8s16o16_6x32) \
	KMACRO(U8S8S32OS32, lpgemm_rowvar_u8s8s32o32_6x64) \
	KMACRO(F32F32F32OF32, lpgemm_rowvar_f32f32f32of32_avx512_6x64m) \
	KMACRO(BF16BF16F32OF32, lpgemm_rowvar_bf16bf16f32of32_6x64) \
	KMACRO(S8S8S32OS32, lpgemm_rowvar_s8s8s32os32_6x64) \
	KMACRO(S8S8S16OS16, lpgemm_rowvar_s8s8s16o16_6x32) \

#define LPGEMM_PACKA_FUNC_MAP_AVX512_VNNI_BF16 \
	PAMACRO(U8S8S16OS16, NULL) \
	PAMACRO(U8S8S32OS32, packa_k64_u8s8s32o32) \
	PAMACRO(BF16BF16F32OF32, NULL) \
	PAMACRO(S8S8S32OS32, packa_k64_s8s8s32os32) \
	PAMACRO(S8S8S16OS16, NULL) \

#define LPGEMM_PACKB_FUNC_MAP_AVX512_VNNI_BF16 \
	PBMACRO(U8S8S16OS16, packb_nr32_u8s8s16o16) \
	PBMACRO(U8S8S32OS32, packb_nr64_u8s8s32o32) \
	PBMACRO(BF16BF16F32OF32, packb_nr64_bf16bf16f32of32) \
	PBMACRO(S8S8S32OS32, packb_nr64_s8s8s32os32) \
	PBMACRO(S8S8S16OS16, packb_nr32_s8s8s16o16) \

#define LPGEMM_UTIL_KERN_FUNC_MAP_AVX512_VNNI_BF16 \
	UMACRO(F32_GELU_TANH, lpgemm_util_f32_gelu_tanh_avx512_kernel) \
	UMACRO(F32_GELU_ERF, lpgemm_util_f32_gelu_erf_avx512_kernel) \
	UMACRO(F32_SOFTMAX, lpgemm_util_f32_softmax_avx512_kernel) \

// Icelake
#define LPGEMM_KERN_FUNC_MAP_AVX512_VNNI \
	KMACRO(U8S8S16OS16, lpgemm_rowvar_u8s8s16o16_6x32) \
	KMACRO(U8S8S32OS32, lpgemm_rowvar_u8s8s32o32_6x64) \
	KMACRO(F32F32F32OF32, lpgemm_rowvar_f32f32f32of32_avx512_6x64m) \
	KMACRO(BF16BF16F32OF32, lpgemm_rowvar_bf16bf16f32of32_6x64) \
	KMACRO(S8S8S32OS32, lpgemm_rowvar_s8s8s32os32_6x64) \
	KMACRO(S8S8S16OS16, lpgemm_rowvar_s8s8s16o16_6x32) \

#define LPGEMM_PACKA_FUNC_MAP_AVX512_VNNI \
	PAMACRO(U8S8S16OS16, NULL) \
	PAMACRO(U8S8S32OS32, packa_k64_u8s8s32o32) \
	PAMACRO(BF16BF16F32OF32, NULL) \
	PAMACRO(S8S8S32OS32, packa_k64_s8s8s32os32) \
	PAMACRO(S8S8S16OS16, NULL) \

#define LPGEMM_PACKB_FUNC_MAP_AVX512_VNNI \
	PBMACRO(U8S8S16OS16, packb_nr32_u8s8s16o16) \
	PBMACRO(U8S8S32OS32, packb_nr64_u8s8s32o32) \
	PBMACRO(BF16BF16F32OF32, packb_nr64_bf16bf16f32of32) \
	PBMACRO(S8S8S32OS32, packb_nr64_s8s8s32os32) \
	PBMACRO(S8S8S16OS16, packb_nr32_s8s8s16o16) \

#define LPGEMM_UTIL_KERN_FUNC_MAP_AVX512_VNNI \
	UMACRO(F32_GELU_TANH, lpgemm_util_f32_gelu_tanh_avx512_kernel) \
	UMACRO(F32_GELU_ERF, lpgemm_util_f32_gelu_erf_avx512_kernel) \
	UMACRO(F32_SOFTMAX, lpgemm_util_f32_softmax_avx512_kernel) \

// Skylake
#define LPGEMM_KERN_FUNC_MAP_AVX512 \
	KMACRO(U8S8S16OS16, lpgemm_rowvar_u8s8s16o16_6x32) \
	KMACRO(U8S8S32OS32, lpgemm_rowvar_u8s8s32o32_6x64) \
	KMACRO(F32F32F32OF32, lpgemm_rowvar_f32f32f32of32_avx512_6x64m) \
	KMACRO(BF16BF16F32OF32, lpgemm_rowvar_bf16bf16f32of32_6x64) \
	KMACRO(S8S8S32OS32, lpgemm_rowvar_s8s8s32os32_6x64) \
	KMACRO(S8S8S16OS16, lpgemm_rowvar_s8s8s16o16_6x32) \

#define LPGEMM_PACKA_FUNC_MAP_AVX512 \
	PAMACRO(U8S8S16OS16, NULL) \
	PAMACRO(U8S8S32OS32, packa_k64_u8s8s32o32) \
	PAMACRO(BF16BF16F32OF32, NULL) \
	PAMACRO(S8S8S32OS32, packa_k64_s8s8s32os32) \
	PAMACRO(S8S8S16OS16, NULL) \

#define LPGEMM_PACKB_FUNC_MAP_AVX512 \
	PBMACRO(U8S8S16OS16, packb_nr32_u8s8s16o16) \
	PBMACRO(U8S8S32OS32, packb_nr64_u8s8s32o32) \
	PBMACRO(BF16BF16F32OF32, packb_nr64_bf16bf16f32of32) \
	PBMACRO(S8S8S32OS32, packb_nr64_s8s8s32os32) \
	PBMACRO(S8S8S16OS16, packb_nr32_s8s8s16o16) \

#define LPGEMM_UTIL_KERN_FUNC_MAP_AVX512 \
	UMACRO(F32_GELU_TANH, lpgemm_util_f32_gelu_tanh_avx512_kernel) \
	UMACRO(F32_GELU_ERF, lpgemm_util_f32_gelu_erf_avx512_kernel) \
	UMACRO(F32_SOFTMAX, lpgemm_util_f32_softmax_avx512_kernel) \

// Milan, Haswell
#define LPGEMM_KERN_FUNC_MAP_AVX2 \
	KMACRO(U8S8S16OS16, lpgemm_rowvar_u8s8s16o16_6x32) \
	KMACRO(U8S8S32OS32, NULL) \
	KMACRO(F32F32F32OF32, lpgemm_rowvar_f32f32f32of32_6x16m) \
	KMACRO(BF16BF16F32OF32, NULL) \
	KMACRO(S8S8S32OS32, NULL) \
	KMACRO(S8S8S16OS16, lpgemm_rowvar_s8s8s16o16_6x32) \

#define LPGEMM_PACKA_FUNC_MAP_AVX2 \
	PAMACRO(U8S8S16OS16, NULL) \
	PAMACRO(U8S8S32OS32, NULL) \
	PAMACRO(BF16BF16F32OF32, NULL) \
	PAMACRO(S8S8S32OS32, NULL) \
	PAMACRO(S8S8S16OS16, NULL) \

#define LPGEMM_PACKB_FUNC_MAP_AVX2 \
	PBMACRO(U8S8S16OS16, packb_nr32_u8s8s16o16) \
	PBMACRO(U8S8S32OS32, NULL) \
	PBMACRO(BF16BF16F32OF32, NULL) \
	PBMACRO(S8S8S32OS32, NULL) \
	PBMACRO(S8S8S16OS16, packb_nr32_s8s8s16o16) \

#define LPGEMM_UTIL_KERN_FUNC_MAP_AVX2 \
	UMACRO(F32_GELU_TANH, lpgemm_util_f32_gelu_tanh_avx2_kernel) \
	UMACRO(F32_GELU_ERF, lpgemm_util_f32_gelu_erf_avx2_kernel) \
	UMACRO(F32_SOFTMAX, lpgemm_util_f32_softmax_avx2_kernel) \

#endif //LPGEMM_FUNC_MAP_H
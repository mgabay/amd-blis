/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2022-2023, Advanced Micro Devices, Inc. All rights reserved.

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

#ifndef LPGEMM_CONFIG_H
#define LPGEMM_CONFIG_H

#include "lpgemm_types.h"

// equals to number of ops in enum AOCL_OPERATION_TYPE.
extern lpgemm_cntx_t lpgemm_global_cntx_t_list[AOCL_OPERATION_TYPE_LEN];
extern lpgemm_cntx_t lpgemm_util_global_cntx_t_list[AOCL_UTIL_OPERATION_TYPE_LEN];

void aocl_lpgemm_init_global_cntx();

lpgemm_cntx_t* lpgemm_get_global_cntx_obj( AOCL_OPERATION_TYPE op );

lpgemm_util_cntx_t* lpgemm_util_get_global_cntx_obj( AOCL_UTIL_OPERATION_TYPE op );

dim_t lpgemm_get_block_size_MC_global_cntx( AOCL_OPERATION_TYPE op_type );

dim_t lpgemm_get_block_size_NC_global_cntx( AOCL_OPERATION_TYPE op_type );

dim_t lpgemm_get_block_size_KC_global_cntx( AOCL_OPERATION_TYPE op_type );

dim_t lpgemm_get_block_size_NR_global_cntx( AOCL_OPERATION_TYPE op_type );

dim_t lpgemm_get_block_size_MR_global_cntx( AOCL_OPERATION_TYPE op_type );

void lpgemm_get_packa_strides( lpgemm_cntx_t* lcntx, dim_t* rs, dim_t* cs );

void lpgemm_get_packb_strides( lpgemm_cntx_t* lcntx, dim_t* rs, dim_t* cs );

void lpgemm_mod_block_size_s16
     (
       dim_t  m,
       dim_t  n,
       dim_t  k,
       dim_t* MC,
       dim_t* NC,
       dim_t* KC
     );

#endif //LPGEMM_CONFIG_H

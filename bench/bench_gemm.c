/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas at Austin
   Copyright (C) 2020, Advanced Micro Devices, Inc. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas nor the names of its
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

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "blis.h"


// Benchmark application to process aocl logs generated by BLIS library.
#ifndef DT
#define DT BLIS_DOUBLE
#endif

#ifndef IND
#define IND BLIS_NAT
#endif


#ifndef N_REPEAT
//#define N_REPEAT 100
#endif


#define AOCL_MATRIX_INITIALISATION

//#define BLIS_ENABLE_CBLAS

/* For BLIS since logs are collected at BLAS interfaces
 * we disable cblas interfaces for this benchmark application
 */

/* #ifdef BLIS_ENABLE_CBLAS */
/* #define CBLAS */
/* #endif */

int main( int argc, char** argv )
{
    obj_t a, b, c;
    obj_t c_save;
    obj_t alpha, beta;
    dim_t m, n, k;
    dim_t  p_inc = 0; // to keep track of number of inputs
    num_t dt;
    //    ind_t    ind;
    char     dt_ch;
    int   r, n_repeats;
    trans_t  transa;
    trans_t  transb;

    double   dtime;
    double   dtime_save;
    double   gflops;

    FILE* fin  = NULL;
    FILE* fout = NULL;

    n_repeats = N_REPEAT;  // This macro will get from Makefile.

    dt = DT;

    if (argc < 3)
      {
        printf("Usage: ./test_gemm_XX.x input.csv output.csv\n");
        exit(1);
      }
    fin = fopen(argv[1], "r");
    if (fin == NULL)
      {
        printf("Error opening the file %s\n", argv[1]);
        exit(1);
      }
    fout = fopen(argv[2], "w");
    if (fout == NULL)
      {
        printf("Error opening output file %s\n", argv[2]);
        exit(1);
      }
  fprintf(fout, "Dt m\t n\t k\t lda\t ldb\t ldc\t rs_a rs_b rs_c transa transb alphaR\t alphaI\t betaR\t betaI\t gflops\n");


    // {S,D,C,Z} {m n k cs_a cs_b cs_c rs_a rs_b rs_c transa transb alpha_real alpha_imaginary beta_real beta_imaginary}

    inc_t lda;
    inc_t ldb;
    inc_t ldc;

    char stor_scheme, transA_c, transB_c;
    double alpha_r, beta_r, alpha_i, beta_i;
    dim_t m_trans;
    dim_t n_trans;
    char tmp[256]; // to store function name, line no present in logs.
    dim_t rs_a, rs_b, rs_c;


    //    printf ("inp - %d %c %ld %ld %ld %ld %ld %ld %ld %ld %ld %c %c %lf %lf %lf %lf\n", p_inc++, dt_ch, m, n, k, lda, ldb, ldc, rs_a, rs_b, rs_c, transA_c, transB_c,  alpha_r, alpha_i, beta_r, beta_i);



    // {S,D,C,Z} {m n k cs_a cs_b cs_c rs_a rs_b rs_c transa transb alpha_real alpha_imaginary beta_real beta_imaginary}
    stor_scheme = 'C'; // since logs are collected at BLAS APIs

    while (fscanf(fin, "%s %c %ld %ld %ld %ld %ld %ld %ld %ld %ld %c %c %lf %lf %lf %lf\n", tmp, &dt_ch, &m, &n, &k, &lda,\
                  &ldb, &ldc, &rs_a, &rs_b, &rs_c, &transA_c, &transB_c,  &alpha_r, &alpha_i, &beta_r, &beta_i) == 17)
      {
        if (dt_ch == 'D' || dt_ch == 'd') dt = BLIS_DOUBLE;
        else if (dt_ch == 'Z' || dt_ch == 'z') dt = BLIS_DCOMPLEX;
        else if (dt_ch == 'S' || dt_ch == 's') dt = BLIS_FLOAT;
        else if (dt_ch == 'C' || dt_ch == 'c') dt = BLIS_SCOMPLEX;
        else
          {
            printf("Invalid data type %c\n", dt_ch);
            continue;
          }

        if      (transA_c == 'n' || transA_c == 'N')  transa = BLIS_NO_TRANSPOSE;
        else if (transA_c == 't' || transA_c == 'T')  transa = BLIS_TRANSPOSE;
        else if ( transA_c == 'c' || transA_c == 'C') transa = BLIS_CONJ_TRANSPOSE;
        else
          {
            printf("Invalid option for transA \n");
            continue;
          }

        if      ( transB_c == 'n' || transB_c == 'N') transb = BLIS_NO_TRANSPOSE;
        else if ( transB_c == 't' || transB_c == 'T') transb = BLIS_TRANSPOSE;
        else if ( transB_c == 'c' || transB_c == 'C') transb = BLIS_CONJ_TRANSPOSE;
        else
          {
            printf("Invalid option for transB \n");
            continue;
          }

        bli_obj_create( dt, 1, 1, 0, 0, &alpha);
        bli_obj_create( dt, 1, 1, 0, 0, &beta );

        if( (stor_scheme == 'C') || (stor_scheme == 'c') )
          {
            // Column storage
            // leading dimension should be greater than number of rows

            // The below condition is incorrect.

            // if ((m > lda) || (k > ldb) || (m > ldc)) continue;
            // Since this bench app is run on logs generated by AOCL trace logs
            // - we have relaxed the checks on the input parameters.

            // if A is transpose - A(lda x m), lda = max(1,k)
            // if A is non-transpose - A (lda x k), lda = max(1,m)
            // if B is transpose - B (ldb x k), ldb = max(1,n)
            // if B is non-transpose - B (ldb x n), ldb = max(1,k)
            //    C is ldc x n - ldc = max(1, m)

            bli_set_dims_with_trans( transa, m, k, &m_trans, &n_trans);
            bli_obj_create( dt, m_trans, n_trans, 1, lda, &a);

            bli_set_dims_with_trans( transb, k, n, &m_trans, &n_trans);
            bli_obj_create( dt, m_trans, n_trans, 1, ldb, &b);

            bli_obj_create( dt, m, n, 1, ldc, &c);
            bli_obj_create( dt, m, n, 1, ldc, &c_save );
          }
        else if( (stor_scheme == 'r') || (stor_scheme == 'R') )
          {
            // Row-major order
            //leading dimension should be greater than number of columns
            if ((k > lda) || (n > ldb) || (n > ldc)) continue;

            bli_set_dims_with_trans(transa, m, k, &m_trans, &n_trans);
            bli_obj_create( dt, m_trans, n_trans, lda, 1, &a);

            bli_set_dims_with_trans(transb, k, n, &m_trans, &n_trans);
            bli_obj_create( dt, m_trans, n_trans, ldb, 1, &b);

            bli_obj_create( dt, m, n, ldc, 1, &c);
            bli_obj_create( dt, m, n, ldc, 1, &c_save );
          }
        else
          {
            printf("Invalid storage scheme\n");
            continue;
          }

#ifndef CBLAS
        if(bli_obj_col_stride(&c) == 1)
          {
            printf("BLAS APIs doesn't support row-storage\n");
            continue;
          }
#endif

#ifdef AOCL_MATRIX_INITIALISATION
        bli_randm( &a );
        bli_randm( &b );
        bli_randm( &c );
#endif

        bli_obj_set_conjtrans( transa, &a);
        bli_obj_set_conjtrans( transb, &b);

        bli_setsc( alpha_r, alpha_i, &alpha );
        bli_setsc( beta_r, beta_i, &beta );

        bli_copym( &c, &c_save );

        dtime_save = DBL_MAX;

        for ( r = 0; r < n_repeats; ++r )
          {
            bli_copym( &c_save, &c );

#ifdef PRINT
            bli_printm( "a", &a, "%4.1f", "" );
            bli_printm( "b", &b, "%4.1f", "" );
            bli_printm( "c", &c, "%4.1f", "" );
#endif
            dtime = bli_clock();

#ifdef BLIS

            bli_gemm( &alpha,
                      &a,
                      &b,
                      &beta,
                      &c );

#else

#ifdef CBLAS
            enum CBLAS_ORDER     cblas_order;
            enum CBLAS_TRANSPOSE cblas_transa;
            enum CBLAS_TRANSPOSE cblas_transb;

            if ( bli_obj_row_stride( &c ) == 1 )
              cblas_order = CblasColMajor;
            else
              cblas_order = CblasRowMajor;

            if( bli_is_trans( transa ) )
              cblas_transa = CblasTrans;
            else if( bli_is_conjtrans( transa ) )
              cblas_transa = CblasConjTrans;
            else
              cblas_transa = CblasNoTrans;

            if( bli_is_trans( transb ) )
              cblas_transb = CblasTrans;
            else if( bli_is_conjtrans( transb ) )
              cblas_transb = CblasConjTrans;
            else
              cblas_transb = CblasNoTrans;
#else
            f77_char f77_transa;
            f77_char f77_transb;

            bli_param_map_blis_to_netlib_trans( transa, &f77_transa );
            bli_param_map_blis_to_netlib_trans( transb, &f77_transb );

#endif
            if ( bli_is_float( dt ) )
              {
                f77_int  mm     = bli_obj_length( &c );
                f77_int  kk     = bli_obj_width_after_trans( &a );
                f77_int  nn     = bli_obj_width( &c );


                float*   alphap = bli_obj_buffer( &alpha );
                float*   ap     = bli_obj_buffer( &a );
                float*   bp     = bli_obj_buffer( &b );
                float*   betap  = bli_obj_buffer( &beta );
                float*   cp     = bli_obj_buffer( &c );
#ifdef CBLAS
                cblas_sgemm( cblas_order,
                             cblas_transa,
                             cblas_transb,
                             mm,
                             nn,
                             kk,
                             *alphap,
                             ap, lda,
                             bp, ldb,
                             *betap,
                             cp, ldc
                             );

#else
                sgemm_( &f77_transa,
                        &f77_transb,
                        &mm,
                        &nn,
                        &kk,
                        alphap,
                        ap, (f77_int*)&lda,
                        bp, (f77_int*)&ldb,
                        betap,
                        cp, (f77_int*)&ldc );
#endif
              }
            else if ( bli_is_double( dt ) )
              {
                f77_int  mm     = bli_obj_length( &c );
                f77_int  kk     = bli_obj_width_after_trans( &a );
                f77_int  nn     = bli_obj_width( &c );

                double*  alphap = bli_obj_buffer( &alpha );
                double*  ap     = bli_obj_buffer( &a );
                double*  bp     = bli_obj_buffer( &b );
                double*  betap  = bli_obj_buffer( &beta );
                double*  cp     = bli_obj_buffer( &c );
#ifdef CBLAS
                cblas_dgemm( cblas_order,
                             cblas_transa,
                             cblas_transb,
                             mm,
                             nn,
                             kk,
                             *alphap,
                             ap, lda,
                             bp, ldb,
                             *betap,
                             cp, ldc
                             );

#else
                dgemm_( &f77_transa,
                        &f77_transb,
                        &mm,
                        &nn,
                        &kk,
                        alphap,
                        ap, (f77_int*)&lda,
                        bp, (f77_int*)&ldb,
                        betap,
                        cp, (f77_int*)&ldc );
#endif
              }
            else if ( bli_is_scomplex( dt ) )
              {
                f77_int  mm     = bli_obj_length( &c );
                f77_int  kk     = bli_obj_width_after_trans( &a );
                f77_int  nn     = bli_obj_width( &c );

                scomplex*  alphap = bli_obj_buffer( &alpha );
                scomplex*  ap     = bli_obj_buffer( &a );
                scomplex*  bp     = bli_obj_buffer( &b );
                scomplex*  betap  = bli_obj_buffer( &beta );
                scomplex*  cp     = bli_obj_buffer( &c );

#ifdef CBLAS
                cblas_cgemm( cblas_order,
                             cblas_transa,
                             cblas_transb,
                             mm,
                             nn,
                             kk,
                             alphap,
                             ap, lda,
                             bp, ldb,
                             betap,
                             cp, ldc
                             );

#else
                cgemm_( &f77_transa,
                        &f77_transb,
                        &mm,
                        &nn,
                        &kk,
                        alphap,
                        ap, (f77_int*)&lda,
                        bp, (f77_int*)&ldb,
                        betap,
                        cp, (f77_int*)&ldc );
#endif
              }
            else if ( bli_is_dcomplex( dt ) )
              {
                f77_int  mm     = bli_obj_length( &c );
                f77_int  kk     = bli_obj_width_after_trans( &a );
                f77_int  nn     = bli_obj_width( &c );

                dcomplex*  alphap = bli_obj_buffer( &alpha );
                dcomplex*  ap     = bli_obj_buffer( &a );
                dcomplex*  bp     = bli_obj_buffer( &b );
                dcomplex*  betap  = bli_obj_buffer( &beta );
                dcomplex*  cp     = bli_obj_buffer( &c );
#ifdef CBLAS
                cblas_zgemm( cblas_order,
                             cblas_transa,
                             cblas_transb,
                             mm,
                             nn,
                             kk,
                             alphap,
                             ap, lda,
                             bp, ldb,
                             betap,
                             cp, ldc
                             );

#else
                zgemm_( &f77_transa,
                        &f77_transb,
                        &mm,
                        &nn,
                        &kk,
                        alphap,
                        ap, (f77_int*)&lda,
                        bp, (f77_int*)&ldb,
                        betap,
                        cp, (f77_int*)&ldc );
#endif
              }
#endif

#ifdef PRINT
            bli_printm( "c after", &c, "%4.1f", "" );
            exit(1);
#endif

            dtime_save = bli_clock_min_diff( dtime_save, dtime );
          }

        gflops = ( 2.0 * m * k * n ) / ( dtime_save * 1.0e9 );

        if ( bli_is_complex( dt ) ) gflops *= 4.0;

        printf( "data_gemm_%s", BLAS );

        p_inc++;
        printf("( %2lu, 1:4 ) = [ %4lu %4lu %4lu %7.2f ];\n",
               (unsigned long)(p_inc),
               (unsigned long)m,
               (unsigned long)n,
               (unsigned long)k, gflops);

        fprintf (fout, "%c %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld %ld %ld %c %c %lf\t %lf\t %lf\t %lf\t %6.3f\n", \
                 dt_ch, m, n, k, lda, ldb, ldc, rs_a, rs_b, rs_c, \
                 transA_c, transB_c,  alpha_r, alpha_i, beta_r, beta_i, gflops);

        fflush(fout);

        bli_obj_free( &alpha );
        bli_obj_free( &beta );

        bli_obj_free( &a );
        bli_obj_free( &b );
        bli_obj_free( &c );
        bli_obj_free( &c_save );
      }

    //bli_finalize();
    fclose(fin);
    fclose(fout);

    return 0;
}
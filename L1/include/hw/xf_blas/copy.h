/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file copy.h
 * @brief BLAS Level 1 copy template function implementation.
 *
 * This file is part of XF BLAS Library.
 */

#ifndef XF_BLAS_COPY_H
#define XF_BLAS_COPY_H


#ifndef __cplusplus
#error "BLAS Library only works with C++."
#endif

#include "ap_int.h"
#include "hls_math.h"
#include "hls_stream.h"
#include "xf_blas/utility.h"

namespace xf {
namespace linear_algebra {
namespace blas {
  /**
   * @brief copy function taht compute Y = X
   *
   * @tparam t_DataType the data type of the vector entries
   * @tparam t_DataWidth the number of bits used to represent the datatype
   * @tparam t_ParEntries number of parallelly processed entries in the packed input vector stream
   * @tparam t_IndexType the datatype of the index
   *
   * @param p_n the number of entries in vector X and Y
   * @param p_x the packed input vector stream
   * @param p_y the packed output vector stream 
   */
  template<
    typename t_DataType,
    unsigned int t_DataWidth,
    unsigned int t_ParEntries,
    typename t_IndexType=unsigned int
  >
  void copy (
    unsigned int p_n,
    hls::stream<WideType<t_DataType, t_ParEntries, t_DataWidth> > &p_x,
    hls::stream<WideType<t_DataType, t_ParEntries, t_DataWidth> > &p_y
  ) {
    #ifndef __SYNTHESIS__
    assert((p_n % t_ParEntries) == 0);
    #endif
    const unsigned int l_parEntries = p_n / t_ParEntries;
    for (t_IndexType i=0; i<l_parEntries; ++i) {
    #pragma HLS PIPELINE
      WideType<t_DataType, t_ParEntries, t_DataWidth> l_x;
      l_x = p_x.read();
      p_y.write(l_x);
    }
  } 
} //end namespace blas
} //end namspace linear_algebra
} //end namespace xf

#endif

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

#include "amin.h"
using namespace xf::linear_algebra::blas;
void amin_top(
  unsigned int p_n,
  hls::stream<ap_uint<BLAS_dataWidth * BLAS_parEntries> > &p_x,
  BLAS_indexType &p_result 
) {
  xf::linear_algebra::blas::amin <
    BLAS_dataType,
    BLAS_parEntries,
    BLAS_indexType,
    BLAS_dataWidth
  > (
    p_n, p_x, p_result
  );
}

void readVec2Stream(
  BLAS_dataType p_in[BLAS_size],
  unsigned int p_n,
  hls::stream<ap_uint<BLAS_dataWidth * BLAS_parEntries> > &p_out
) {
  #ifndef __SYNTHESIS__
    assert ((p_n % BLAS_parEntries) == 0);
  #endif
  unsigned int l_parBlocks = p_n / BLAS_parEntries;
  for (unsigned int i=0; i<l_parBlocks; ++i) {
  #pragma HLS PIPELINE
    BitConv<BLAS_dataType> l_bitConv;
    ap_uint<BLAS_dataWidth * BLAS_parEntries> l_val;
    for (unsigned int j=0; j<BLAS_parEntries; ++j) {
      ap_uint<BLAS_dataWidth> l_valUnit = l_bitConv.toBits(p_in[i*BLAS_parEntries + j]);
      l_val = (l_val <<  BLAS_dataWidth) + l_valUnit;
    }
    p_out.write(l_val);
  }
}

void UUT_Top(
  BLAS_dataType p_in[BLAS_size],
  unsigned int p_n,
  BLAS_indexType &p_result
){
  hls::stream<ap_uint<BLAS_dataWidth * BLAS_parEntries> > l_str;
  #pragma HLS DATAFLOW
  readVec2Stream(
    p_in,
    p_n,
    l_str
  );
  amin_top(
    p_n,
    l_str,
    p_result 
  );
}

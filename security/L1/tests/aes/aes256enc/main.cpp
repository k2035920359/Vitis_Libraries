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

#include <iostream>
#include "test.hpp"

using namespace std;

void prt(ap_uint<8>* ptr, int size) {
    for (int i = 0; i < size; i++) {
        cout << hex << *(ptr + size);
    }
    cout << endl;
}

int main() {
    std::cout << "This is an example test for AES256, mentioned in Appendix C.3, "
                 "NIST.FIPS.197.pdf"
              << std::endl;

    // ap_uint<8> plaintext[16] = {0xf0, 0xf1, 0x52, 0x53, 0xa4, 0x55, 0x66, 0x77,
    //                            0xf8, 0xf9, 0x5a, 0x5b, 0xac, 0xdd, 0xee, 0xff};
    ap_uint<8> plaintext[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    ap_uint<8> cipherkey[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    };

    ap_uint<8> ciphertext[16];

    ap_uint<128>* p;
    ap_uint<256>* ck;
    ap_uint<128> cp;

    p = (ap_uint<128>*)plaintext;
    ck = (ap_uint<256>*)cipherkey;
    // for (int i = 0; i < 16; i++) {
    // p(i * 8 + 7, i * 8) = plaintext[15 - i](7, 0);
    // }
    // for (int i = 0; i < 32; i++) {
    // ck(i * 8 + 7, i * 8) = cipherkey[31 - i](7, 0);
    // }

    hls::stream<ap_uint<128> > plaintext_strm;
    hls::stream<bool> i_e_strm;
    hls::stream<ap_uint<256> > cipherkey_strm;
    hls::stream<ap_uint<128> > ciphertext_strm;
    hls::stream<bool> o_e_strm;

    plaintext_strm << *p;
    i_e_strm << false;
    i_e_strm << true;
    cipherkey_strm << *ck;

    test(plaintext_strm, i_e_strm, cipherkey_strm, ciphertext_strm, o_e_strm);
    while (!o_e_strm.read()) {
        ciphertext_strm >> cp;
    }

    for (int i = 0; i < 16; i++) {
        ciphertext[i](7, 0) = cp(i * 8 + 7, i * 8);
    }

    cout << "Plaintext:" << hex << *p << endl;
    cout << "Key:" << hex << *ck << endl;
    cout << "Ciphertext:" << hex << cp << endl;

    return 0;
}

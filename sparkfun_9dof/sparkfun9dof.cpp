#include "sparkfun9dof.h"

void decoder(byte* data, int size, double* results, BitOrder order) {
  for (int i = 0; i < size; ++i) {
    int pos = i * 2;
    int first = (order == LSB_MSB) ? pos + 1 : pos;
    int second = (order == LSB_MSB) ? pos : pos + 1;
    
    int twosComplement = data[first] << 8 | data[second];
    if (twosComplement & 0x8000) {
      // is a negative number
      int sumLSBs = twosComplement & 0x7fff;
      results[i] = (sumLSBs - 0x8000);
    } else {
      // is a positive number
      results[i] = twosComplement;
    }
  }
}

